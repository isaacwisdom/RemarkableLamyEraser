#ifndef ACTIONS_H
#define ACTIONS_H

#include <linux/input.h>
#include <stdbool.h>

//erase modes
#define PRESS_MODE_RM2        1 //press and hold mode
#define TOGGLE_MODE_RM2       2 //toggle mode

#define PRESS_MODE_RM1        3 //press and hold mode for RM1
#define TOGGLE_MODE_RM1       4 //toggle mode for RM1

//actions:
//buttons do not support press and holds, but support clicks.
#define UNDO            1 //undo button
#define REDO            2 //redo button

//static const struct input_event tool_touch_off = { .type = EV_KEY, .code =BTN_TOUCH, .value = 0}; //these might be used in the future to improve press and hold mode
//static const struct input_event tool_pen_on = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1}; //used when pen approaches the screen
//static const struct input_event tool_pen_off = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value =0};
static const struct input_event tool_rubber_on = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; // used when rubber approaches the screen
static const struct input_event tool_rubber_off = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};

bool doublePressHandler(struct input_event ev_pen);

void toggleMode(struct input_event ev_pen, int fd_pen);
void pressMode(struct input_event ev_pen, int fd_pen);

void toggleModeRM1(struct input_event ev_pen, int fd_touch);
void pressModeRM1(struct input_event ev_pen, int fd_touch);

void writeEvent(int fd_touch, struct input_event event);
void writeTapWithTouch(int fd_touch, const int location[2]);

void actionUndo(int fd_touch);
void actionRedo(int fd_touch);



#endif // ACTIONS_H
