#ifndef EFFECTS_DATA_H
#define EFFECTS_DATA_H

#define WACOM   0
#define TOUCH   1

#define NUM_EFFECTS     13
//effects:
#define NULL_EFFECT     0 //null effect
#define TOOLBAR         1 //action: toolbar
#define WRITING         2 //action: writing
#define STROKE          3 //tap on stroke panel, not useful on it's own
#define COLOR_BLACK     4 //action: switch to color black
#define COLOR_GREY      5 //action: switch to color grey
#define COLOR_WHITE     6 //action: switch to color white
#define ERASER_PANEL    7 //tap on eraser panel, not useful on it's own
#define ERASER_ERASE    8 //tool:   eraser normal eraser button
#define ERASER_SELECT   9 //tool:   erase-selection: erase selection button
#define SELECT         10 //tool:   selection
#define UNDO           11 //action: undo button
#define REDO           12 //action: redo button

//define offsets for the effect functions
#define HOLD_OFF_OFFSET      0x0ff
#define TOGGLE_OFFSET        0xfff

//effects that require a deactivate/toggle function
#define ERASER_ERASE_OFF     (ERASER_ERASE  + HOLD_OFF_OFFSET)
#define ERASER_SELECT_OFF    (ERASER_SELECT + HOLD_OFF_OFFSET)
#define SELECT_OFF           (SELECT        + HOLD_OFF_OFFSET)

#define ERASER_ERASE_TOGGLE  (ERASER_ERASE  + TOGGLE_OFFSET)
#define ERASER_SELECT_TOGGLE (ERASER_SELECT + TOGGLE_OFFSET)
#define SELECT_TOGGLE        (SELECT        + TOGGLE_OFFSET)


//recognized words in config file
static const char* effects[NUM_EFFECTS] = {
  "null",               //null effect      0
  "toolbar",            //TOOLBAR          1
  "writing",            //WRITING          2
  "stroke-panel",       //STROKE           3
  "color-black",        //COLOR_BLACK      4
  "color-grey",         //COLOR_GREY       5
  "color-white",        //COLOR_WHITE      6
  "erase-panel",        //ERASER           7
  "eraser",             //ERASER_ERASE     8
  "erase-selection",    //ERASE_SELECTION  9
  "select",             //SELECT          10
  "undo",               //UNDO            11
  "redo",               //REDO            12
};


#define T_RM1_RHY 670 //define RM1 toolbar column touchscreen location
#define T_RM1_RHX 530
#define T_RM1_LHY 780
#define T_RM1_LHX 520
#define T_RM2_RHX 60  //define RM2 toolbar column touchscreen location
#define T_RM2_RHY 50
#define T_RM2_LHX 1360
#define T_RM2_LHY 1820
//                                  [rmVersion][EFFECT][ORIENTATION][x/y]
static const int locationLookupTouch[2][NUM_EFFECTS][4][2] = {
  { //RM1
    //RHP                //RHL              //LHP              //LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NO_EFFECT      0
    { {T_RM1_RHX,  990}, { 720, T_RM1_RHY}, {T_RM1_LHX,  990}, { 720, T_RM1_LHY} }, //TOOLBAR        1
    { {T_RM1_RHX,  930}, { 670, T_RM1_RHY}, {T_RM1_LHX,  930}, { 670, T_RM1_LHY} }, //WRITING        2
    { {T_RM1_RHX,  870}, { 600, T_RM1_RHY}, {T_RM1_LHX,  870}, { 600, T_RM1_LHY} }, //STROKE         3
    { {      760,  610}, { 340,       150}, {      150,  610}, { 340,       860} }, //COLOR_BLACK    4
    { {      760,  550}, { 380,       150}, {      150,  550}, { 380,       860} }, //COLOR_GREY     5
    { {      760,  490}, { 320,       150}, {      150,  490}, { 320,       860} }, //COLOR_WHITE    6
    { {T_RM1_RHX,  800}, { 530, T_RM1_RHY}, {T_RM1_LHX,  800}, { 530, T_RM1_LHY} }, //ERASER_PANEL   7
    { {      760,  800}, { 530,       150}, {      150,  800}, { 530,       860} }, //ERASER_ERASE   8
    { {      760,  720}, { 480,       150}, {      150,  730}, { 460,       860} }, //ERASER_SELECT  9
    { {T_RM1_RHX,  720}, { 480, T_RM1_RHY}, {T_RM1_LHX,  720}, { 460, T_RM1_LHY} }, //SELECT        10
    { {T_RM1_RHX,  660}, { 400, T_RM1_RHY}, {T_RM1_LHX,  660}, { 400, T_RM1_LHY} }, //UNDO          11
    { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //REDO          12
  },
  { //RM2
    //RHP                  //RHL                //LHP                //LHL
    { {        0,    0}, {    0,         0}, {        0,    0}, {    0,         0} }, //NO_EFFECT      0
    { {T_RM2_RHX, 1820}, {   70, T_RM2_RHY}, {T_RM2_LHX, 1820}, {   70, T_RM2_LHY} }, //TOOLBAR        1
    { {T_RM2_RHX, 1680}, {  200, T_RM2_RHY}, {T_RM2_LHX, 1680}, {  200, T_RM2_LHY} }, //WRITING        2
    { {T_RM2_RHX, 1570}, {  320, T_RM2_RHY}, {T_RM2_LHX, 1570}, {  320, T_RM2_LHY} }, //STROKE         3
    { {      280, 1120}, {  770,       270}, {     1140, 1120}, {  770,      1590} }, //COLOR_BLACK    4
    { {      280,  970}, {  920,       270}, {     1140,  970}, {  920,      1590} }, //COLOR_GREY     5
    { {      280,  880}, { 1031,       270}, {     1140,  880}, { 1031,      1590} }, //COLOR_WHITE    6
    { {T_RM2_RHX, 1450}, {  430, T_RM2_RHY}, {T_RM2_LHX, 1450}, {  430, T_RM2_LHY} }, //ERASER_PANEL   7
    { {      280, 1450}, {  430,       270}, {     1140, 1450}, {  430,      1590} }, //ERASER_ERASE   8
    { {      280, 1330}, {  550,       270}, {     1140, 1340}, {  550,      1590} }, //ERASER_SELECT  9
    { {T_RM2_RHX, 1330}, {  550, T_RM2_RHY}, {T_RM2_LHX, 1340}, {  550, T_RM2_LHY} }, //SELECT        10
    { {T_RM2_RHX, 1220}, {  670, T_RM2_RHY}, {T_RM2_LHX, 1220}, {  670, T_RM2_LHY} }, //UNDO          11
    { {T_RM2_RHX, 1090}, {  800, T_RM2_RHY}, {T_RM2_LHX, 1090}, {  800, T_RM2_LHY} }, //REDO          12
  }
};

#define W_RM2_RHY 730 //define toolbar column wacom location
#define W_RM2_RHX 530
#define W_RM2_LHY 15210
#define W_RM2_LHX 20260
//                                  [EFFECT][ORIENTATION][x/y]
static const int locationLookupWacom[NUM_EFFECTS][4][2] = {
    //RHP                  //RHL                //LHP                //LHL
    { {     0,          0}, {         0,     0}, {     0,          0}, {   0,           0} }, //NO_EFFECT      0
    { { 20290,  W_RM2_RHY}, { W_RM2_RHX,   700}, { 20300,  W_RM2_LHY}, { W_RM2_LHX,   680} }, //TOOLBAR        1
    { { 18930,  W_RM2_RHY}, { W_RM2_RHX,  1900}, { 18920,  W_RM2_LHY}, { W_RM2_LHX,  1910} }, //WRITING        2
    { { 17600,  W_RM2_RHY}, { W_RM2_RHX,  3500}, { 17600,  W_RM2_LHY}, { W_RM2_LHX,  3500} }, //STROKE         3
    { { 12400,       3000}, {      3000,  8600}, { 12400,      12500}, {     17900,  8600} }, //COLOR_BLACK    4
    { { 11100,       3000}, {      3000,  9700}, { 11100,      12500}, {     17900,  9700} }, //COLOR_GREY     5
    { {  9900,       3000}, {      3000, 11200}, {  9900,      12500}, {     17900, 11200} }, //COLOR_WHITE    6
    { { 16130,  W_RM2_RHY}, { W_RM2_RHX,  4630}, { 16030,  W_RM2_LHY}, { W_RM2_LHX,  4780} }, //ERASER_PANEL   7
    { { 16100,       3000}, {      3000,  4740}, { 16090,      12500}, {     17900,  4810} }, //ERASER_ERASE   8
    { { 14950,       3000}, {      3000,  5950}, { 14880,      12500}, {     17900,  6070} }, //ERASER_SELECT  9
    { { 14820,  W_RM2_RHY}, { W_RM2_RHX,  5950}, { 14900,  W_RM2_LHY}, { W_RM2_LHX,  6180} }, //SELECT        10
    { { 13380,  W_RM2_RHY}, { W_RM2_RHX,  7610}, { 13350,  W_RM2_LHY}, { W_RM2_LHX,  7450} }, //UNDO          11
    { { 12220,  W_RM2_RHY}, { W_RM2_RHX,  8880}, { 12300,  W_RM2_LHY}, { W_RM2_LHX,  8780} }, //REDO          12
//}
};


#endif //EFFECTS_DATA_H
