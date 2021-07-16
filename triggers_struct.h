#ifndef TRIGGERS_STRUCT_H
#define TRIGGERS_STRUCT_H

#include <stdbool.h>
#include <linux/input.h>


//triggers:
//Click        || Press&Hold
//Double Click || DoublePress&Hold
//Triple Click || TripleClick&Hold


typedef struct {
  unsigned char type;   //click or press and hold (click = 0, press&hold = 1)
  unsigned char state;  //on or off
} triggerCode;

typedef struct {
  triggerCode   code;
  unsigned char number; //number
} triggerType;


#define TRIGGER_OFF_2    0
#define TRIGGER_ON_2     1
#define CLICK_2          0
#define PRESS_HOLD_2     1

#define CODE_CLICK_2          (triggerCode) {CLICK_2, TRIGGER_ON_2};
#define CODE_PRESS_HOLD_ON_2  (triggerCode) {PRESS_HOLD_2, TRIGGER_ON_2};
#define CODE_PRESS_HOLD_OFF_2 (triggerCode) {PRESS_HOLD_2, TRIGGER_OFF_2};
#define TRIGGER_NULL_2   (triggerType) {{0,0}, 0}

triggerType getTrigger2(struct input_event* ev_pen);
void printTriggers(triggerType* trigger, bool includeNull);

#endif // TRIGGERS_H
