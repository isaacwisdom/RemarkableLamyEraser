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
  static bool           sent = 0;
  static struct timeval prevTime;
  static struct timeval abortTime;
  static bool           abort;
  static struct timeval possiblyReleasedTime;
  static bool           possiblyReleased;
  static bool           possiblyLongClick;
  static bool           longClick;

  int trigger = NULL_TRIGGER;

  if (abort) {
    if (!(ev_pen->code == EV_SYN || ev_pen->code == BTN_STYLUS)) {
      abort = false;

      double elapsedTime = getTimeDelta(&(ev_pen->time), &abortTime);
      if (elapsedTime > MAX_CYCLE_TIME) {
        // the pen moved away from the screen, and has just re-approached; abort and reset state
        printf("Aborting trigger\n");
        clickRegistered = false;
        possiblyLongClick = false;
        possiblyReleased = false;
        longClick = false;
        if (sent) {
          trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
          sent    = false;
        }
        clicks  = 0;
      }
    }
  }

  if (ev_pen->code == BTN_TOOL_PEN && ev_pen->value == 0) {
    abortTime = ev_pen->time;
    abort = true;
  }

  bool released = false;

  if (ev_pen->code != EV_SYN && possiblyReleased) {
    possiblyReleased = false;

    double elapsedTime = getTimeDelta(&(ev_pen->time), &possiblyReleasedTime);
    if (elapsedTime < MAX_CYCLE_TIME) {
      released = true;
    }
    else if (sent) { // otherwise, it was a press-and-pull-away
      trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
      clicks  = 0;
      sent    = false;
    }
  }

  if (ev_pen->code == BTN_STYLUS && ev_pen->value == 0) {
    // don't register release immediately to ignore spurious signals
    // when moving pen away from screen with button pressed
    possiblyReleasedTime = ev_pen->time;
    possiblyReleased = true;
  }

  if (sent && ev_pen->code == ABS_PRESSURE) possiblyLongClick = false; // abort long click if pen touches screen

  if (ev_pen->code == BTN_STYLUS && ev_pen->value == 1) {
    prevTime = ev_pen->time; // update most recent time
    clicks += 1;
    // don't set the trigger as we don't have enough
    // info to ascertain the state yet.
    clickRegistered = false;
  }

  if (longClick) {
    printf("Long click triggered\n");
    trigger = 0x00 | clicks; // long click type message 0b00xxxxxx
    clicks = 0;
    longClick = false;
    possiblyLongClick = false;
  }

  if (clicks > 0) {
    double elapsedTime = getTimeDelta(&(ev_pen->time), &prevTime);  // time between presses of button
    if (elapsedTime < MAX_CLICK_TIME) {
      if (released) {
        printf("Click Detected\n");
        // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
        clickRegistered = true;
      }
    } else if (elapsedTime < MAX_DOUBLE_CLICK_TIME) { // between MCT and MCDT
      if (!clickRegistered) { // button still held or just released
        if (!sent) {
          // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
          trigger = 0xc0 | clicks; // press hold on type message 0b11xxxxxx
          sent    = true;
          possiblyLongClick = true;
        }

        if (released) {
          // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
          trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
          sent    = false;
          if (possiblyLongClick) longClick = true; // send a long click in the next cycle
          else clicks = 0;
        }
      }
    } else { // after MDCT
      if (clickRegistered) {
        // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
        trigger       = 0x80 | clicks; // click type message 0b10xxxxxx
        clickRegistered = false;
        clicks        = 0;
      }
      if (released) {
        // printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n" "elapsedTime = %f, clickRegistered = %d\n\n", clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec, elapsedTime, clickRegistered);
        trigger = 0x40 | clicks; // press hold off type message 0b01xxxxxx
        sent    = false;
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
