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

#define CLICK        0x80

#define LCLICK       0x10

#define HOLD_ON      0xc0

#define HOLD_OFF     0x40

#define PEN_UP       0x50

#define PEN_DOWN     0x60

#define MAX_CYCLE_TIME         0.01 //seconds
#define MAX_CONTACT_CLICK_TIME 0.01 //seconds
#define MAX_CLICK_TIME         0.2  //seconds
#define MAX_DOUBLE_CLICK_TIME  0.4  //seconds

int get_triggger(struct input_event *ev_pen);

void print_triggers(int trigger, bool include_null);

#endif // TRIGGERS_H
