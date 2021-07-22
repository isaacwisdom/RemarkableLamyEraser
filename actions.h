#ifndef ACTIONS_H
#define ACTIONS_H

#include <linux/input.h>
#include <stdbool.h>

#include "orientation.h"

#define WACOM   0
#define TOUCH   1

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

//     event1                    event2
//RM2: Pen (0,0) in Bottom Left, Touchscreen (0,0) in Bottom Left
//     event0                    event2
//RM1: Pen (0,0) in Bottom Left, Touchscreen (0,0) in Bottom Right

//Touch Screen Coordinates:
#define RM1_RHtoolX 720 //define one x coordinate for all panel tools
#define RM1_RHtoolY 35 //define one y coordinate for panel tools
#define RM1_LHtoolX 20 //define one y coordinate for panel tools
#define RM1_LHtoolY 990 //define one y coordinate for panel tools

#define RM2_RHtoolX 60 //define one x coordinate for all panel tools
#define RM2_RHtoolY 50 //define one y coordinate for panel tools
#define RM2_LHtoolX 1360 //define one y coordinate for panel tools
#define RM2_LHtoolY 1820 //define one y coordinate for panel tools

#define T_RM1_RHY 670
#define T_RM1_RHX 530
#define T_RM1_LHY 780
#define T_RM1_LHX 520

#define T_RM2_RHX 60
#define T_RM2_RHY 50
#define T_RM2_LHX 1360
#define T_RM2_LHY 1820

static const int locationLookup[2][NUM_EFFECTS][4][2] = {
  { /* RM1 */
    //RHP                //RHL              //LHP              //LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NO_EFFECT      0
    { {T_RM1_RHX,  990}, { 720, T_RM1_RHY}, {T_RM1_LHX,  990}, { 720, T_RM1_LHY} }, //TOOLBAR        1
    { {T_RM1_RHX,  930}, { 670, T_RM1_RHY}, {T_RM1_LHX,  920}, { 660, T_RM1_LHY} }, //WRITING        2
    { {T_RM1_RHX,  800}, { 530, T_RM1_RHY}, {T_RM1_LHX,  790}, { 530, T_RM1_LHY} }, //ERASER_PANEL   3
    { {      760,  790}, { 530,       150}, {     150,   790}, { 530,       860} }, //ERASER_ERASE   4
    { {      760,  720}, { 480,       150}, {     150,   730}, { 460,       860} }, //ERASER_SELECT  5
    { {T_RM1_RHX,  720}, { 480, T_RM1_RHY}, {T_RM1_LHX,  720}, { 460, T_RM1_LHY} }, //SELECT         6
    { {T_RM1_RHX,  660}, { 400, T_RM1_RHY}, {T_RM1_LHX,  660}, { 400, T_RM1_LHY} }, //UNDO           7
    { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //REDO           8
  },
  { /* RM2 */
    //RHP                  //RHL                //LHP                //LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NO_EFFECT      0
    { {T_RM2_RHX, 1820}, {  70, T_RM2_RHY}, {T_RM2_LHX, 1820}, {  70, T_RM2_LHY} }, //TOOLBAR        1
    { {T_RM2_RHX, 1680}, { 200, T_RM2_RHY}, {T_RM2_LHX, 1680}, { 200, T_RM2_LHY} }, //WRITING        2
    { {T_RM2_RHX, 1450}, { 430, T_RM2_RHY}, {T_RM2_LHX, 1450}, { 430, T_RM2_LHY} }, //ERASER_PANEL   3
    { {      280, 1450}, { 430,       270}, {     1140, 1450}, { 430,      1590} }, //ERASER_ERASE   4
    { {      280, 1330}, { 550,       270}, {     1140, 1340}, { 550,      1590} }, //ERASER_SELECT  5
    { {T_RM2_RHX, 1330}, { 550, T_RM2_RHY}, {T_RM2_LHX, 1340}, { 550, T_RM2_LHY} }, //SELECT         6
    { {T_RM2_RHX, 1220}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1220}, { 670, T_RM2_LHY} }, //UNDO           7
    { {T_RM2_RHX, 1090}, { 800, T_RM2_RHY}, {T_RM2_LHX, 1090}, { 800, T_RM2_LHY} }, //REDO           8
  }
};

#define W_RM1_RHY 680
#define W_RM1_RHX 710
#define W_RM1_LHY 15050
#define W_RM1_LHX 20260

#define W_RM2_RHY 730
#define W_RM2_RHX 530
#define W_RM2_LHY 15210
#define W_RM2_LHX 20260

static const int locationLookupWacom[2][NUM_EFFECTS][4][2] = {
  { /* RM1 */
    //RHP                  //RHL                //LHP                //LHL
    { {     0,          0}, {         0,    0}, {     0,          0}, {   0,          0} }, //NO_EFFECT      0
    { { 20280,  W_RM1_RHY}, { W_RM1_RHX,  670}, { 20220,  W_RM1_LHY}, { W_RM1_LHX,  700} }, //TOOLBAR        1
    { { 19030,  W_RM1_RHY}, { W_RM1_RHX, 1950}, { 18990,  W_RM1_LHY}, { W_RM1_LHX, 2090} }, //WRITING        2
    { { 16220,  W_RM1_RHY}, { W_RM1_RHX, 4660}, { 16220,  W_RM1_LHY}, { W_RM1_LHX, 4610} }, //ERASER_PANEL   3
    { { 16150,       2860}, {      3110, 4730}, { 16240,      12650}, {     18050, 4820} }, //ERASER_ERASE   4
    { { 14890,       2860}, {      3110, 4730}, { 14920,      12650}, {     18050, 6150} }, //ERASE_SELECT   5
    { { 14820,  W_RM1_RHY}, { W_RM1_RHX, 5860}, { 14880,  W_RM1_LHY}, { W_RM1_LHX, 6110} }, //SELECT         6
    { { 13440,  W_RM1_RHY}, { W_RM1_RHX, 5860}, { 14880,  W_RM1_LHY}, { W_RM1_LHX, 7330} }, //UNDO           7
    { { 12280,  W_RM1_RHY}, { W_RM1_RHX, 5860}, { 12150,  W_RM1_LHY}, { W_RM1_LHX, 8810} }, //REDO           8
  },
  { /* RM2 */
    //RHP                  //RHL                //LHP                //LHL
    { {     0,          0}, {         0,    0}, {     0,          0}, {   0,          0} }, //NO_EFFECT      0
    { { 20290,  W_RM2_RHY}, { W_RM2_RHX,  700}, { 20300,  W_RM2_LHY}, { W_RM2_LHX,  680} }, //TOOLBAR        1
    { { 18930,  W_RM2_RHY}, { W_RM2_RHX, 1900}, { 18920,  W_RM2_LHY}, { W_RM2_LHX, 1910} }, //WRITING        2
    { { 16130,  W_RM2_RHY}, { W_RM2_RHX, 4630}, { 16030,  W_RM2_LHY}, { W_RM2_LHX, 4780} }, //ERASER_PANEL   3
    { { 16100,       2730}, {      3090, 4740}, { 16090,      12470}, {     17920, 4810} }, //ERASER_ERASE   4
    { { 14950,       2730}, {      3090, 5950}, { 14880,      12470}, {     17920, 6070} }, //ERASER_SELECT  5
    { { 14820,  W_RM2_RHY}, { W_RM2_RHX, 5950}, { 14900,  W_RM2_LHY}, { W_RM2_LHX, 6180} }, //SELECT         6
    { { 13380,  W_RM2_RHY}, { W_RM2_RHX, 7610}, { 13350,  W_RM2_LHY}, { W_RM2_LHX, 7450} }, //UNDO           7
    { { 12220,  W_RM2_RHY}, { W_RM2_RHX, 8880}, { 12300,  W_RM2_LHY}, { W_RM2_LHX, 8780} }, //REDO           8
  }
};



//static const struct input_event tool_touch_off = { .type = EV_KEY, .code =BTN_TOUCH, .value = 0}; //these might be used in the future to improve press and hold mode
static const struct input_event tool_pen_on = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1}; //used when pen approaches the screen
static const struct input_event tool_pen_off = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value =0};
static const struct input_event tool_rubber_on = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; // used when rubber approaches the screen
static const struct input_event tool_rubber_off = {.type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};

void writeEvent(int fd_touch, struct input_event event);
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
void actionUndo(int fd_touch, int rmVersion);
void actionRedo(int fd_touch, int rmVersion);




#endif // ACTIONS_H
