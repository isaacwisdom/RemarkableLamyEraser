#include <bits/types/struct_timeval.h>
#include <stdbool.h>
#include <stdio.h>

#include "triggers.h"

inline double getTimeDelta(struct timeval *current, struct timeval *prev) {
  return (current->tv_sec + current->tv_usec / 1000000.0) -
         (prev->tv_sec + prev->tv_usec / 1000000.0);
}

int get_triggger(struct input_event *ev_pen) {
  // triggers:
  // Click        || Press&Hold
  // Double Click || DoublePress&Hold
  // Triple Click || TripleClick&Hold
  // Bit 7 encodes trigger on or off
  // Bit 6 encodes Click or Press and hold
  // Bits 0-5 encodes the number ie. (click, double click, press and hold,
  // double press and hold)

  static int            clicks  = 0;
  static bool           clickRegistered;
  static bool           pressHoldSent = 0;
  static struct timeval prevTime;
  static struct timeval abortTime;
  static bool           abort;
  static bool           contact;
  static struct timeval possiblyReleasedTime;
  static bool           possiblyReleased;
  static bool           possiblyLongClick;
  static bool           longClick;
  static struct timeval possiblyLiftedTime;
  static bool           possiblyLifted;
  static bool           possiblyLiftedGotClick;
  static bool           possiblyLiftedGotContact;

  int trigger = NULL_TRIGGER;

  if (abort) {
    if (!(ev_pen->code == EV_SYN || ev_pen->code == BTN_STYLUS)) { // BTN_TOOL_PEN = 0 is followed by EV_SYN and (possibly) BTN_STYLUS when pulling away pen
      abort = false;

      double elapsedTime = getTimeDelta(&(ev_pen->time), &abortTime);
      if (elapsedTime > MAX_CYCLE_TIME) {
        // the pen moved away from the screen, and has just re-approached; abort and reset state
        // printf("Event: ABORT...\n");
        clickRegistered = false;
        possiblyLongClick = false;
        longClick = false;
        contact = false;
        possiblyLifted = false;
        possiblyLiftedGotContact = false;
        possiblyLiftedGotClick = false;
        if (pressHoldSent) {
          // pen was pressed and later pulled away... can only know to send hold off after pen re-approaches screen
          printf("Event: PEN PRESS-PULL-AWAY...\n");
          trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
          pressHoldSent    = false;
        }
        clicks   = 0;
      }
    }
  }

  if (ev_pen->code == BTN_TOOL_PEN && ev_pen->value == 0) { // indicates that pen was pulled away from screen, initiate abort sequence
    abortTime = ev_pen->time;
    abort = true;
  }

  if (possiblyLifted) {
    double elapsedTime = getTimeDelta(&(ev_pen->time), &possiblyLiftedTime);

    if (elapsedTime > MAX_CONTACT_CLICK_TIME) {

      printf("Event: PEN LIFT...\n");

      possiblyLiftedGotContact = false;
      possiblyLiftedGotClick = false;
      possiblyLifted = false;

      contact = false;
      trigger = PEN_UP; // pen-up type message
    }
    else {
      if (ev_pen->code == BTN_STYLUS && ev_pen->value == 1) {
        // printf("got click\n");
        possiblyLiftedGotClick = true;
      }

      if (ev_pen->code == ABS_DISTANCE && ev_pen->value == 0) {
        // printf("got contact\n");
        possiblyLiftedGotContact = true;
      }

      // recieving button release and distance 0 codes in rapid succession
      // indicates a button click with the pen on screen, not an actual lift
      if (possiblyLiftedGotClick && possiblyLiftedGotContact) {
        // printf("detected fake lift\n");

        // abort, this was a fake lift
        possiblyLiftedGotContact = false;
        possiblyLiftedGotClick = false;
        possiblyLifted = false;
      }
    }
  } else if (contact) {
    if (ev_pen->code == ABS_DISTANCE){
      // printf("Possible lift detected\n");

      // don't register lift immediately to ignore spurious signals
      // when pressing button with pen on screen
      if (!possiblyLifted) possiblyLiftedTime = ev_pen->time;
      possiblyLifted = true;
    }
  } else {
    if (ev_pen->code == ABS_DISTANCE && ev_pen->value == 0) {
      printf("Event: PEN CONTACT...\n");
      contact = true;
    }
  }


  bool released = false;

  if (ev_pen->code != EV_SYN && possiblyReleased) {
    possiblyReleased = false;

    double elapsedTime = getTimeDelta(&(ev_pen->time), &possiblyReleasedTime);
    if (elapsedTime < MAX_CYCLE_TIME) {
      // if we recieved a non-sync code within a short enough delay, this was a genuine button release (i.e. not a press-and-pull-away)
      released = true;
    }
  }

  if (ev_pen->code == BTN_STYLUS && ev_pen->value == 0) {
    // don't register release immediately to ignore spurious signals
    // when moving pen away from screen with button pressed
    possiblyReleasedTime = ev_pen->time;
    possiblyReleased = true;
  }

  if (pressHoldSent && ev_pen->code == ABS_PRESSURE) possiblyLongClick = false; // abort long click if pen touches screen
  // if (ev_pen->code == ABS_PRESSURE) {
  //   printf("pressure: %d\n", ev_pen->value);
  // }

  if (ev_pen->code == BTN_STYLUS && ev_pen->value == 1) {
    if (!contact) {
      prevTime = ev_pen->time; // update most recent time
      clicks += 1;
      // don't set the trigger as we don't have enough
      // info to ascertain the state yet.
      clickRegistered = false;
    } else {
      printf("Event: PEN CONTACT PRESS...\n");
    }
  }

  if (longClick) {
    printf("Event: PEN LONG CLICK (%d)...\n", clicks);
    trigger = 0x00 | clicks; // long click type message 0b00xxxxxx
    clicks = 0;
    longClick = false;
    possiblyLongClick = false;
  }

  if (clicks > 0) {
    double elapsedTime = getTimeDelta(&(ev_pen->time), &prevTime);  // time between presses of button
    if (elapsedTime < MAX_CLICK_TIME) {
      if (released) {
        // printf("Click Detected\n");
        // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
        clickRegistered = true;
      }
    } else if (elapsedTime < MAX_DOUBLE_CLICK_TIME) { // between MCT and MCDT
      if (!clickRegistered) { // button still held or just released
        if (!pressHoldSent) {
          // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
          printf("Event: PEN HOLD START (%d)...\n", clicks);
          trigger = 0xc0 | clicks; // press hold on type message 0b11xxxxxx
          pressHoldSent    = true;
          possiblyLongClick = true; // (long click will be aborted if pen touches screen before button release)
        }
        else if (released) { // edge case: button pressed held and released between MCT and MCDT, pen hold end
          // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
          printf("Event: PEN HOLD END (%d)...\n", clicks);
          trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
          pressHoldSent    = false;
          if (possiblyLongClick) longClick = true; // send a long click in the next cycle
          else clicks = 0;
        }
      }
    } else { // after MDCT
      if (clickRegistered) {
        // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
        printf("Event: PEN CLICK (%d)...\n", clicks);
        trigger       = 0x80 | clicks; // click type message 0b10xxxxxx
        clickRegistered = false;
        clicks        = 0;
      }
      if (released) {
        // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
        printf("Event: PEN HOLD END (%d)...\n", clicks);
        trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
        pressHoldSent    = false;
        if (possiblyLongClick) longClick = true; // send a long click in the next cycle
        else clicks = 0;
      }
    }
  }

  // printf("%x, %d\n", trigger, clickRegistered);
  return trigger;
}

void print_triggers(int trigger, bool include_null) {
  if (trigger) {
    printf("Hex: %x | ", trigger);
    if (trigger & 0b01000000)
      printf("PRESS&HOLD | ");
    else
      printf("CLICK      | ");
    if (trigger & 0b10000000)
      printf("ON  | ");
    else
      printf("OFF | ");
    printf("NUMBER: %d\n", trigger & 0b00111111);
  } else if (include_null) {
    printf("%x : NULL\n", trigger);
  }
}
