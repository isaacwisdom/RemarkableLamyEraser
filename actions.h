#ifndef ACTIONS_H
#define ACTIONS_H

#include <linux/input.h>
#include <stdbool.h>

//erase modes
#define PRESS_MODE      1 //press and hold mode
#define TOGGLE_MODE     2 //toggle mode

//actions:
//buttons do not support press and holds, but support clicks.
#define UNDO                  1 //undo button
#define REDO                  2 //redo button

//static const struct input_event tool_touch_off = { .type = EV_KEY, .code =BTN_TOUCH, .value = 0}; //these might be used in the future to improve press and hold mode
//static const struct input_event tool_pen_on = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1}; //used when pen approaches the screen
//static const struct input_event tool_pen_off = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value =0};
static const struct input_event tool_rubber_on = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; // used when rubber approaches the screen
static const struct input_event tool_rubber_off = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};

void writeEvent(int fd, struct input_event event);
void writeTapWithTouch(int fd, const int location[2]);
void toggleMode(struct input_event ev, int fd);
bool doublePressHandler(struct input_event ev);
void pressMode(struct input_event ev, int fd);
void actionUndo(int fd);
void actionRedo(int fd);

#endif // ACTIONS_H
