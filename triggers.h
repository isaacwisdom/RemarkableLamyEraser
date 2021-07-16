#ifndef TRIGGERS_H
#define TRIGGERS_H

#include <stdbool.h>
#include <linux/input.h>


//triggers:
//Click        || Press&Hold
//Double Click || DoublePress&Hold
//Triple Click || TripleClick&Hold
//Bit 7 encodes trigger on or off
//Bit 6 encodes Click or Press and hold
//Bits 0-5 encodes the number as an int ie. (click=1, double click=2, press and hold=1, double press and hold=2)
#define TRIGGER_NULL              0b00000000

#define CLICK                     0b10000001
#define DOUBLE_CLICK              0b10000010
#define TRIPLE_CLICK              0b10000011

#define PRESS_HOLD_ON             0b11000001
#define DOUBLE_PRESS_HOLD_ON      0b11000010
#define TRIPLE_PRESS_HOLD_ON      0b11000011

#define PRESS_HOLD_OFF            0b01000001
#define DOUBLE_PRESS_HOLD_OFF     0b01000010
#define TRIPLE_PRESS_HOLD_OFF     0b01000011


int getTrigger(struct input_event* ev_pen);
void printTriggers(int trigger, bool includeNull);

#endif // TRIGGERS_H
