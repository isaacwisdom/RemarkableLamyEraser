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

const char* effectStringFromInt(int effect);

void writeEvent(int fd_touch, struct input_event event);
void handleCurrentTrackingID(struct input_event* event);
void writeTapWithTouch(int fd_touch, const int location[2]);
void writeTapWithPen(int fd_touch, const int location[2]);
int  writeOrientedTapSequence(int device, int fd_touch, toolbarOrientation* orientation, int RMversion, int numLocations, ...);



void activateToolEraserRM2(int fd_pen);
void deactivateToolEraserRM2(int fd_pen);
void toggleToolEraserRM2(int fd_pen);
void actionToolEraserRM2(struct input_event* ev_pen, int fd_pen);

void activateToolEraserRM1(int fd_touch, int rmVersion);
void deactivateToolEraserRM1(int fd_touch, int rmVersion);
void toggleToolEraserRM1(int fd_touch, int rmVersion);

void activateToolEraseSelect(int fd_touch, int rmVersion);
void deactivateToolEraseSelect(int fd_touch, int rmVersion);
void toggleToolEraseSelect(int fd_touch, int rmVersion);

void activateToolSelect(int fd_touch, int rmVersion);
void deactivateToolSelect(int fd_touch, int rmVersion);
void toggleToolSelect(int fd_touch, int rmVersion);


void actionToolbar(int fd_touch, int rmVersion);
void actionWriting(int fd_touch, int rmVersion);
void actionColorBlack(int fd_pen, int rmVersion);
void actionColorGrey(int fd_pen, int mVersion);
void actionColorWhite(int fd_pen, int rmVersion);
void actionUndo(int fd_touch, int rmVersion);
void actionRedo(int fd_touch, int rmVersion);

void testLocations(int device, int fd, int rmVersion);



#endif // EFFECTS_H
