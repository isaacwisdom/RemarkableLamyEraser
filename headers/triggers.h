#ifndef TRIGGERS_H
#define TRIGGERS_H

#include <linux/input.h>
#include <stdbool.h>

// Triggers:
//
//   Click        || Press&Hold
//   Double Click || DoublePress&Hold
//   Triple Click || TripleClick&Hold
//   Bit 7 encodes trigger on or off
//   Bit 6 encodes Click or Press and hold
//   Bits 4-5 unused
//   Bits 0-3 encodes the number as an int; i.e.,
//     (click=1, double click=2, press and hold=1, double press and hold=2)

#define NULL_TRIGGER 0x00

#define CLICK_1      0x81
#define CLICK_2      0x82
#define CLICK_3      0x83
#define CLICK_4      0x84
#define CLICK_5      0x85

#define LCLICK_1 0x01
#define LCLICK_2 0x02
#define LCLICK_3 0x03
#define LCLICK_4 0x04
#define LCLICK_5 0x05

#define HOLD_1_ON    0xc1
#define HOLD_2_ON    0xc2
#define HOLD_3_ON    0xc3
#define HOLD_4_ON    0xc4
#define HOLD_5_ON    0xc5

#define HOLD_1_OFF   0x41
#define HOLD_2_OFF   0x42
#define HOLD_3_OFF   0x43
#define HOLD_4_OFF   0x44
#define HOLD_5_OFF   0x45

#define MAX_CYCLE_TIME         0.01 //seconds
#define MAX_CLICK_TIME         0.2  //seconds
#define MAX_DOUBLE_CLICK_TIME  0.4  //seconds

int get_triggger(struct input_event *ev_pen);

void print_triggers(int trigger, bool include_null);

#endif // TRIGGERS_H
