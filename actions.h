#ifndef ACTIONS_H
#define ACTIONS_H

#include <linux/input.h>
#include <stdbool.h>

#include "orientation.h"
//erase modes
#define PRESS_MODE            1
#define TOGGLE_MODE           2

#define PRESS_MODE_RM1        3 //press and hold mode for RM1
#define TOGGLE_MODE_RM1       4 //toggle mode for RM1

#define PRESS_MODE_RM2        5 //press and hold mode
#define TOGGLE_MODE_RM2       6 //toggle mode



#define TOOLBAR         0
//actions:
//instant-types do not support press and holds, but support clicks.
#define WRITING         1 //writing button
#define UNDO            4 //undo button
#define REDO            5 //redo button
//state-types support both press and holds and clicks.
#define ERASER          2 //eraser button
#define SELECT          3 //select button

//Touch Screen Coordinates:
#define RM2_RHtoolX 60 //define one x coordinate for all panel tools
#define RM2_RHtoolY 50 //define one y coordinate for panel tools
#define RM2_LHtoolX 1360 //define one y coordinate for panel tools
#define RM2_LHtoolY 1820 //define one y coordinate for panel tools
static const int locationLookup[2][6][4][2] = {
  { /* RM1 */
  //RHP              //RHL            //LHP            //LHL
  { {RM2_RHtoolX, 1820}, {  40, RM2_RHtoolY}, {RM2_LHtoolX, 1820}, {  40, RM2_LHtoolY} }, //TOOLBAR  0
  { {RM2_RHtoolX, 1680}, {  80, RM2_RHtoolY}, {RM2_LHtoolX, 1680}, {  80, RM2_LHtoolY} }, //WRITING  1
  { {RM2_RHtoolX, 1450}, { 430, RM2_RHtoolY}, {RM2_LHtoolX, 1450}, { 430, RM2_LHtoolY} }, //ERASER   2
  { {RM2_RHtoolX, 1340}, { 550, RM2_RHtoolY}, {RM2_LHtoolX, 1340}, { 550, RM2_LHtoolY} }, //SELECT   3
  { {RM2_RHtoolX, 1220}, { 670, RM2_RHtoolY}, {RM2_LHtoolX, 1220}, { 670, RM2_LHtoolY} }, //UNDO     4
  { {RM2_RHtoolX, 1090}, { 800, RM2_RHtoolY}, {RM2_LHtoolX, 1090}, { 800, RM2_LHtoolY} }, //REDO     5
  },
  { /* RM2 */
  //RHP              //RHL            //LHP            //LHL
  { {RM2_RHtoolX, 1820}, {  40, RM2_RHtoolY}, {RM2_LHtoolX, 1820}, {  40, RM2_LHtoolY} }, //TOOLBAR  0
  { {RM2_RHtoolX, 1680}, {  80, RM2_RHtoolY}, {RM2_LHtoolX, 1680}, {  80, RM2_LHtoolY} }, //WRITING  1
  { {RM2_RHtoolX, 1450}, { 430, RM2_RHtoolY}, {RM2_LHtoolX, 1450}, { 430, RM2_LHtoolY} }, //ERASER   2
  { {RM2_RHtoolX, 1340}, { 550, RM2_RHtoolY}, {RM2_LHtoolX, 1340}, { 550, RM2_LHtoolY} }, //SELECT   3
  { {RM2_RHtoolX, 1220}, { 670, RM2_RHtoolY}, {RM2_LHtoolX, 1220}, { 670, RM2_LHtoolY} }, //UNDO     4
  { {RM2_RHtoolX, 1090}, { 800, RM2_RHtoolY}, {RM2_LHtoolX, 1090}, { 800, RM2_LHtoolY} }, //REDO     5
  }
};


//static const struct input_event tool_touch_off = { .type = EV_KEY, .code =BTN_TOUCH, .value = 0}; //these might be used in the future to improve press and hold mode
//static const struct input_event tool_pen_on = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1}; //used when pen approaches the screen
//static const struct input_event tool_pen_off = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value =0};
static const struct input_event tool_rubber_on = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; // used when rubber approaches the screen
static const struct input_event tool_rubber_off = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};

void writeEvent(int fd_touch, struct input_event event);
void writeTapWithTouch(int fd_touch, const int location[2]);
int  writeOrientedTapSequence(int fd_touch, toolbarOrientation *orientation, int RMversion, int numLocations, ...);

bool doublePressHandler(struct input_event ev_pen);

void toggleModeRM2(struct input_event ev_pen, int fd_pen);
void pressModeRM2(struct input_event ev_pen, int fd_pen);

void toggleModeRM1(struct input_event ev_pen, int fd_touch, int RMversion);
void pressModeRM1(struct input_event ev_pen, int fd_touch, int RMversion);


void actionToolbar(int fd_touch, int RMversion);
void actionWriting(int fd_touch, int RMversion);
void actionUndo(int fd_touch, int RMversion);
void actionRedo(int fd_touch, int RMversion);



#endif // ACTIONS_H
