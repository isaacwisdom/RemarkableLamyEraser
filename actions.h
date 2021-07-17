#ifndef ACTIONS_H
#define ACTIONS_H

#include <linux/input.h>
#include <stdbool.h>

#include "orientation.h"

#define NUM_EFFECTS     9

//effects:
#define NULL_EFFECT     0 //null effect
#define TOOLBAR         1 //action: toolbar
#define WRITING         2 //action: writing
#define ERASER_PANEL    3 //<eraser  (internal use only)>
#define ERASER_ERASE    4 //tool:   eraser normal eraser button
#define ERASER_SELECT    5 //tool:   erase-selection: erase selection button
#define SELECT          6 //tool:   selection
#define UNDO            7 //action: undo button
#define REDO            8 //action: redo button

#define HOLD_OFF_OFFSET      0x0ff
#define ERASER_ERASE_OFF     (ERASER_ERASE +  HOLD_OFF_OFFSET)
#define ERASER_SELECT_OFF     (ERASER_SELECT + HOLD_OFF_OFFSET)
#define SELECT_OFF           (SELECT + HOLD_OFF_OFFSET)

#define TOGGLE_OFFSET        0xfff
#define ERASER_ERASE_TOGGLE  (ERASER_ERASE +  TOGGLE_OFFSET)
#define ERASER_SELECT_TOGGLE  (ERASER_SELECT + TOGGLE_OFFSET)
#define SELECT_TOGGLE        (SELECT + TOGGLE_OFFSET)

//Touch Screen Coordinates:
#define RM2_RHtoolX 60 //define one x coordinate for all panel tools
#define RM2_RHtoolY 50 //define one y coordinate for panel tools
#define RM2_LHtoolX 1360 //define one y coordinate for panel tools
#define RM2_LHtoolY 1820 //define one y coordinate for panel tools
static const int locationLookup[2][NUM_EFFECTS][4][2] = {
  { /* RM1 */
    //RHP                  //RHL                //LHP                //LHL
    { {          0,    0}, {   0,           0}, {          0,    0}, {   0,           0} }, //NO_EFFECT      0
    { {RM2_RHtoolX, 1820}, {  40, RM2_RHtoolY}, {RM2_LHtoolX, 1820}, {  40, RM2_LHtoolY} }, //TOOLBAR        1
    { {RM2_RHtoolX, 1680}, {  80, RM2_RHtoolY}, {RM2_LHtoolX, 1680}, {  80, RM2_LHtoolY} }, //WRITING        2
    { {RM2_RHtoolX, 1450}, { 430, RM2_RHtoolY}, {RM2_LHtoolX, 1450}, { 430, RM2_LHtoolY} }, //ERASER         3
    { {        280, 1450}, { 430,         270}, {       1140, 1450}, { 430,        1590} }, //ERASER_ERASE   4
    { {        280, 1330}, { 550,         270}, {       1140, 1340}, { 550,        1590} }, //ERASE_SELECT   5
    { {RM2_RHtoolX, 1330}, { 550, RM2_RHtoolY}, {RM2_LHtoolX, 1340}, { 550, RM2_LHtoolY} }, //SELECT         6
    { {RM2_RHtoolX, 1220}, { 670, RM2_RHtoolY}, {RM2_LHtoolX, 1220}, { 670, RM2_LHtoolY} }, //UNDO           7
    { {RM2_RHtoolX, 1090}, { 800, RM2_RHtoolY}, {RM2_LHtoolX, 1090}, { 800, RM2_LHtoolY} }, //REDO           8
  },
  { /* RM2 */
    //RHP                  //RHL                //LHP                //LHL
    { {          0,    0}, {   0,           0}, {          0,    0}, {   0,           0} }, //NO_EFFECT      0
    { {RM2_RHtoolX, 1820}, {  40, RM2_RHtoolY}, {RM2_LHtoolX, 1820}, {  40, RM2_LHtoolY} }, //TOOLBAR        1
    { {RM2_RHtoolX, 1680}, {  80, RM2_RHtoolY}, {RM2_LHtoolX, 1680}, {  80, RM2_LHtoolY} }, //WRITING        2
    { {RM2_RHtoolX, 1450}, { 430, RM2_RHtoolY}, {RM2_LHtoolX, 1450}, { 430, RM2_LHtoolY} }, //ERASER         3
    { {        280, 1450}, { 430,         270}, {       1140, 1450}, { 430,        1590} }, //ERASER_ERASE   4
    { {        280, 1330}, { 550,         270}, {       1140, 1340}, { 550,        1590} }, //ERASE_SELECT   5
    { {RM2_RHtoolX, 1330}, { 550, RM2_RHtoolY}, {RM2_LHtoolX, 1340}, { 550, RM2_LHtoolY} }, //SELECT         6
    { {RM2_RHtoolX, 1220}, { 670, RM2_RHtoolY}, {RM2_LHtoolX, 1220}, { 670, RM2_LHtoolY} }, //UNDO           7
    { {RM2_RHtoolX, 1090}, { 800, RM2_RHtoolY}, {RM2_LHtoolX, 1090}, { 800, RM2_LHtoolY} }, //REDO           8
  }
};



//static const struct input_event tool_touch_off = { .type = EV_KEY, .code =BTN_TOUCH, .value = 0}; //these might be used in the future to improve press and hold mode
static const struct input_event tool_pen_on = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1}; //used when pen approaches the screen
static const struct input_event tool_pen_off = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value =0};
static const struct input_event tool_rubber_on = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; // used when rubber approaches the screen
static const struct input_event tool_rubber_off = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};

void writeEvent(int fd_touch, struct input_event event);
void writeTapWithTouch(int fd_touch, const int location[2]);
int  writeOrientedTapSequence(int fd_touch, toolbarOrientation* orientation, int RMversion, int numLocations, ...);



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
void actionUndo(int fd_touch, int rmVersion);
void actionRedo(int fd_touch, int rmVersion);




#endif // ACTIONS_H
