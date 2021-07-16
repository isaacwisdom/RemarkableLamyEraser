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
#define WRITING         1 //writing button
#define ERASER          2 //eraser button
#define ERASER_ERASE    3 //eraser: normal eraser button
#define ERASE_SELECT    4 //eraser: erase selection button
#define SELECT          5 //select button
#define UNDO            6 //undo button
#define REDO            7 //redo button

//Touch Screen Coordinates:
#define RM2_RHtoolX 60 //define one x coordinate for all panel tools
#define RM2_RHtoolY 50 //define one y coordinate for panel tools
#define RM2_LHtoolX 1360 //define one y coordinate for panel tools
#define RM2_LHtoolY 1820 //define one y coordinate for panel tools
static const int locationLookup[2][8][4][2] = {
  { /* RM1 */
    //RHP                  //RHL                //LHP                //LHL
    { {RM2_RHtoolX, 1820}, {  40, RM2_RHtoolY}, {RM2_LHtoolX, 1820}, {  40, RM2_LHtoolY} }, //TOOLBAR        0
    { {RM2_RHtoolX, 1680}, {  80, RM2_RHtoolY}, {RM2_LHtoolX, 1680}, {  80, RM2_LHtoolY} }, //WRITING        1
    { {RM2_RHtoolX, 1450}, { 430, RM2_RHtoolY}, {RM2_LHtoolX, 1450}, { 430, RM2_LHtoolY} }, //ERASER         2
    { {        280, 1450}, { 430,         270}, {       1140, 1450}, { 430,        1590} }, //ERASER_ERASE   3
    { {        280, 1330}, { 550,         270}, {       1140, 1340}, { 550,        1590} }, //ERASE_SELECT   4
    { {RM2_RHtoolX, 1330}, { 550, RM2_RHtoolY}, {RM2_LHtoolX, 1340}, { 550, RM2_LHtoolY} }, //SELECT         5
    { {RM2_RHtoolX, 1220}, { 670, RM2_RHtoolY}, {RM2_LHtoolX, 1220}, { 670, RM2_LHtoolY} }, //UNDO           6
    { {RM2_RHtoolX, 1090}, { 800, RM2_RHtoolY}, {RM2_LHtoolX, 1090}, { 800, RM2_LHtoolY} }, //REDO           7
  },
  { /* RM2 */
    //RHP                  //RHL                //LHP                //LHL
    { {RM2_RHtoolX, 1820}, {  40, RM2_RHtoolY}, {RM2_LHtoolX, 1820}, {  40, RM2_LHtoolY} }, //TOOLBAR        0
    { {RM2_RHtoolX, 1680}, {  80, RM2_RHtoolY}, {RM2_LHtoolX, 1680}, {  80, RM2_LHtoolY} }, //WRITING        1
    { {RM2_RHtoolX, 1450}, { 430, RM2_RHtoolY}, {RM2_LHtoolX, 1450}, { 430, RM2_LHtoolY} }, //ERASER         2
    { {        280, 1450}, { 430,         270}, {       1140, 1450}, { 430,        1590} }, //ERASER_ERASE   3
    { {        280, 1330}, { 550,         270}, {       1140, 1340}, { 550,        1590} }, //ERASE_SELECT   4
    { {RM2_RHtoolX, 1330}, { 550, RM2_RHtoolY}, {RM2_LHtoolX, 1340}, { 550, RM2_LHtoolY} }, //SELECT         5
    { {RM2_RHtoolX, 1220}, { 670, RM2_RHtoolY}, {RM2_LHtoolX, 1220}, { 670, RM2_LHtoolY} }, //UNDO           6
    { {RM2_RHtoolX, 1090}, { 800, RM2_RHtoolY}, {RM2_LHtoolX, 1090}, { 800, RM2_LHtoolY} }, //REDO           7
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

void activateToolEraseSelection(int fd_touch, int rmVersion);
void deactivateToolEraseSelection(int fd_touch, int rmVersion);
void toggleToolEraseSelection(int fd_touch, int rmVersion);

void activateToolSelect(int fd_touch, int rmVersion);
void deactivateToolSelect(int fd_touch, int rmVersion);
void toggleToolSelect(int fd_touch, int rmVersion);


void actionToolbar(int fd_touch, int rmVersion);
void actionWriting(int fd_touch, int rmVersion);
void actionUndo(int fd_touch, int rmVersion);
void actionRedo(int fd_touch, int rmVersion);




#endif // ACTIONS_H
