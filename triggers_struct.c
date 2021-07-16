#include <stdio.h>
#include <stdbool.h>


#include "triggers_struct.h"


double getTimeDelta(struct timeval* current, struct timeval* prev) {
  return (current->tv_sec + current->tv_usec / 1000000.0) - (prev->tv_sec + prev->tv_usec / 1000000.0);
}


#define MAX_CLICK_TIME         0.15  //seconds
#define MAX_DOUBLE_CLICK_TIME  0.4  //seconds

triggerType getTrigger2(struct input_event* ev_pen) {
  //triggers:
  //Click        || Press&Hold
  //Double Click || DoublePress&Hold
  //Triple Click || TripleClick&Hold

  static double elapsedTime; //time between presses of button
  static triggerType trigger = TRIGGER_NULL_2;
  static unsigned char clicks = 0;
  static bool clickDetected;
  static bool sent = 0;
  static struct timeval prevTime;


if(ev_pen->code == BTN_STYLUS && ev_pen->value == 1) {
    prevTime = ev_pen->time; //update most recent time
    clicks += 1 ;
    trigger = TRIGGER_NULL_2; //set the trigger to NULL as we don't have enough info to ascertain the state yet.
    clickDetected = false;
    }


if (clicks > 0) {
    elapsedTime = getTimeDelta(&(ev_pen->time), &prevTime);
    if (elapsedTime < MAX_CLICK_TIME) {
        if (ev_pen->code == BTN_STYLUS && ev_pen->value == 0) {
            //printf("Click Detected\n");
          /*printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n"
                   "elapsedTime = %f, clickDeteced = %d\n\n",
                   clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec,
                   elapsedTime, clickDetected); */
            clickDetected = true;
          }
      }
    else if (elapsedTime < MAX_DOUBLE_CLICK_TIME) { //between MCT and MCDT
      if (!clickDetected) {
          if (!sent) {
          /*printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n"
                   "elapsedTime = %f, clickDeteced = %d\n\n",
                   clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec,
                   elapsedTime, clickDetected); */
            trigger.code = CODE_PRESS_HOLD_ON_2;
            trigger.number = clicks; //press hold on type message 0b11xxxxxx
            sent = true;
          }
          else
            trigger = TRIGGER_NULL_2;

          if (ev_pen->code == BTN_STYLUS && ev_pen->value == 0) {
              /*printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n"
                     "elapsedTime = %f, clickDeteced = %d\n\n",
                     clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec,
                     elapsedTime, clickDetected); */
              trigger.code = CODE_PRESS_HOLD_OFF_2;
              trigger.number = clicks; //press hold off type message 0b01xxxxxx
              clicks = 0;
              sent = false;
          }
        }
      }
    else {//after MDCT
        if (clickDetected) {
            /*printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n"
                   "elapsedTime = %f, clickDeteced = %d\n\n",
                   clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec,
                   elapsedTime, clickDetected);*/
            trigger.code = CODE_CLICK_2;
            trigger.number = clicks;//click type message 0b10xxxxxx
            clickDetected = false;
            clicks = 0;
            }
        if (ev_pen->code == BTN_STYLUS && ev_pen->value == 0) {
            /*printf("Clicks: %d, prevTime=%ld.%ld, now=%ld.%ld\n"
                   "elapsedTime = %f, clickDeteced = %d\n\n",
                   clicks, prevTime.tv_sec, prevTime.tv_usec, ev_pen->time.tv_sec, ev_pen->time.tv_usec,
                   elapsedTime, clickDetected);*/
            trigger.code = CODE_PRESS_HOLD_OFF_2;
            trigger.number = clicks;
            clicks = 0;
            sent = false;
            }
      }
  }
else
  trigger = TRIGGER_NULL_2;

return trigger;
}

void printTriggers(triggerType* trigger, bool includeNull) {
  if ( !(trigger->code.type || trigger->code.state  || trigger->number) == 0 ) {
      printf("%d.%d.%d: ", trigger->code.type, trigger->code.state, trigger->number);
      if (trigger->code.type == CLICK_2)
        printf("CLICK      | ");
      else
        printf("PRESS&HOLD | ");
      if (trigger->code.state == TRIGGER_ON_2)
        printf("ON  | ");
      else
        printf("OFF | ");
      printf ("NUMBER: %d\n", trigger->number);
    }
  else if (includeNull) {
      printf("%d.%d.%d: ", trigger->code.type, trigger->code.state, trigger->number);
      printf("NULL\n");
  }
}

