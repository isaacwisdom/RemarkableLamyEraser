#ifndef EFFECTS_H
#define EFFECTS_H

#include <linux/input.h>
#include <stdbool.h>

#include "effects_data.h"
#include "orientation.h"

//static const struct input_event tool_touch_off = {.type = EV_KEY, .code = BTN_TOUCH,       .value = 0}; //these might be used in the future to improve press and hold mode
static const struct input_event tool_pen_on =      {.type = EV_KEY, .code = BTN_TOOL_PEN,    .value = 1}; //used when pen approaches the screen
static const struct input_event tool_pen_off =     {.type = EV_KEY, .code = BTN_TOOL_PEN,    .value = 0};
static const struct input_event tool_rubber_on =   {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; // used when rubber approaches the screen
static const struct input_event tool_rubber_off =  {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};

void write_event(int fd_touch, struct input_event event);
void handle_current_tracking_ID(struct input_event *event);
void write_tap_with_touch(int fd_touch, const int location[2]);
int  write_oriented_tap_sequence(int fd_touch, int num_locations, ...);

void activate_tool_eraser(int fd_pen);
void deactivate_tool_eraser(int fd_pen);
void toggle_tool_eraser(int fd_pen);
void action_tool_eraser(struct input_event *ev_pen, int fd_pen);

void activate_tool_eraser_select(int fd_touch);
void deactivate_tool_eraser_select(int fd_touch);
void toggle_tool_eraser_select(int fd_touch);

void activate_tool_select(int fd_touch);
void deactivate_tool_select(int fd_touch);
void toggle_tool_select(int fd_touch);

void action_toolbar(int fd_touch);
void action_writing(int fd_touch);
void action_stroke_panel(int fd_touch);
void action_text(int fd_touch);
void action_eraser_panel(int fd_touch);
void action_undo(int fd_touch);
void action_redo(int fd_touch);
void action_fineliner(int fd_touch);
void action_calligraphy(int fd_touch);
void action_black(int fd_touch);
void action_grey(int fd_touch);
void action_hl(int fd_touch);
void toggle_hl(int fd_touch);

void test_locations(int fd);

#endif // EFFECTS_H
