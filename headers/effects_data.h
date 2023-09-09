#ifndef EFFECTS_DATA_H
#define EFFECTS_DATA_H

#define WACOM   0
#define TOUCH   1

//[DOCTYPE]
#define NOTEBOOK        0
#define PDF             1


#define NUM_EFFECTS     11
//[EFFECT]:
#define NULL_EFFECT       0 //null effect
#define TOOLBAR           1 //action: toolbar
#define WRITING           2 //action: writing
#define TEXT              3 //action: text button
#define ERASER_PANEL      4 //tap on eraser panel, not useful on it's own
#define SELECT            5 //action: select
#define UNDO              6 //action: undo button
#define REDO              7 //action: redo button
#define ERASER_ERASE      8 //action: eraser
#define ERASER_SELECTION  9 //action: eraser-select
#define ERASER_ERASE_ALL 10 //action: eraser-eraser-all

//define offsets for the effect functions
#define HOLD_OFF_OFFSET      0x0ff
#define TOGGLE_OFFSET        0xfff

//effects that require a deactivate/toggle function
#define ERASER_ERASE_OFF     (ERASER_ERASE     + HOLD_OFF_OFFSET)
#define ERASER_SELECTION_OFF (ERASER_SELECTION + HOLD_OFF_OFFSET)
#define SELECT_OFF           (SELECT           + HOLD_OFF_OFFSET)

#define ERASER_ERASE_TOGGLE  (ERASER_ERASE     + TOGGLE_OFFSET)
#define ERASER_SELECT_TOGGLE (ERASER_SELECTION + TOGGLE_OFFSET)
#define SELECT_TOGGLE        (SELECT           + TOGGLE_OFFSET)

//recognized words in config file
static const char* effects[NUM_EFFECTS] = {
  "null-effect",        //null effect        0
  "toolbar",            //TOOLBAR            1
  "writing",            //WRITING            2
  "text",               //TEXT               3
  "eraser-panel",       //ERASER             4
  "select",             //SELECT             5
  "undo",               //UNDO               6
  "redo",               //REDO               7
  "eraser-erase",       //ERASER_ERASE       8
  "eraser-selection",   //ERASER_SELECT      9
  "eraser-erase-all"    //ERASER_ERASE_ALL  10
};

#define T_RM2_RHX 60  // Define RM2 toolbar column touchscreen location
#define T_RM2_RHY 50
#define T_RM2_LHX 1360
#define T_RM2_LHY 1820

static const int locationLookupTouch[2][NUM_EFFECTS][4][2] = {
  {//NOTEBOOK
   // RHP                RHL                LHP                LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT       0
    { {T_RM2_RHX, 1850}, {  70, T_RM2_RHY}, {T_RM2_LHX, 1810}, {  80, T_RM2_LHY} }, //TOOLBAR           1
    { {T_RM2_RHX, 1720}, { 150, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING           2
    { {T_RM2_RHX, 1620}, { 360, T_RM2_RHY}, {T_RM2_LHX, 1500}, { 370, T_RM2_LHY} }, //TEXT              3
    { {T_RM2_RHX, 1510}, { 450, T_RM2_RHY}, {T_RM2_LHX, 1400}, { 480, T_RM2_LHY} }, //ERASER_PANEL      4
    { {T_RM2_RHX, 1410}, { 570, T_RM2_RHY}, {T_RM2_LHX, 1230}, { 590, T_RM2_LHY} }, //SELECT            5
    { {T_RM2_RHX, 1300}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1200}, { 690, T_RM2_LHY} }, //UNDO              6
    { {T_RM2_RHX, 1190}, { 770, T_RM2_RHY}, {T_RM2_LHX,    0}, { 790, T_RM2_LHY} }, //REDO              7
    { {      160, 1470}, { 470,       220}, {     1140,    0}, { 480,      1600} }, //ERASER_ERASE      8
    { {      280, 1470}, { 570,       220}, {     1140,    0}, { 580,      1600} }, //ERASER_SELECTION  9
    { {      280, 1200}, { 660,       220}, {     1140,    0}, { 670,      1600} }, //ERASER_ERASE_ALL 10
  },
  {//PDF
   // RHP                RHL                LHP                LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT       0
    { {T_RM2_RHX, 1850}, {  70, T_RM2_RHY}, {T_RM2_LHX, 1810}, {  80, T_RM2_LHY} }, //TOOLBAR           1
    { {T_RM2_RHX, 1720}, { 150, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING           2
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //TEXT              3
    { {T_RM2_RHX, 1410}, { 450, T_RM2_RHY}, {T_RM2_LHX, 1400}, { 480, T_RM2_LHY} }, //ERASER_PANEL      4
    { {T_RM2_RHX, 1300}, { 570, T_RM2_RHY}, {T_RM2_LHX, 1230}, { 590, T_RM2_LHY} }, //SELECT            5
    { {T_RM2_RHX, 1190}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1200}, { 690, T_RM2_LHY} }, //UNDO              6
    { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //REDO              7
    { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_ERASE      8
    { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_SELECTION  9
    { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_ERASE_ALL 10
  }
};

#define W_RM2_RHY 730    // define toolbar column wacom location
#define W_RM2_RHX 530
#define W_RM2_LHY 15210
#define W_RM2_LHX 20260
                                  //[EFFECT][ORIENTATION][x/y]
static const int locationLookupWacom[NUM_EFFECTS][4][2] = {
    //RHP                  //RHL                //LHP                //LHL
    { {     0,          0}, {         0,     0}, {     0,          0}, {   0,           0} }, //NO_EFFECT      0
    { { 20290,  W_RM2_RHY}, { W_RM2_RHX,   700}, { 20300,  W_RM2_LHY}, { W_RM2_LHX,   680} }, //TOOLBAR        1
    { { 18930,  W_RM2_RHY}, { W_RM2_RHX,  1900}, { 18920,  W_RM2_LHY}, { W_RM2_LHX,  1910} }, //WRITING        2
    { { 17600,  W_RM2_RHY}, { W_RM2_RHX,  3500}, { 17600,  W_RM2_LHY}, { W_RM2_LHX,  3500} }, //STROKE         3
    { { 16130,  W_RM2_RHY}, { W_RM2_RHX,  4630}, { 16030,  W_RM2_LHY}, { W_RM2_LHX,  4780} }, //ERASER_PANEL   4
    { { 14820,  W_RM2_RHY}, { W_RM2_RHX,  5950}, { 14900,  W_RM2_LHY}, { W_RM2_LHX,  6180} }, //SELECT         5
    { { 13380,  W_RM2_RHY}, { W_RM2_RHX,  7610}, { 13350,  W_RM2_LHY}, { W_RM2_LHX,  7450} }, //UNDO           6
    { { 12220,  W_RM2_RHY}, { W_RM2_RHX,  8880}, { 12300,  W_RM2_LHY}, { W_RM2_LHX,  8780} }, //REDO           7
};


#endif //EFFECTS_DATA_H
