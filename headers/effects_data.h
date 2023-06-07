#ifndef EFFECTS_DATA_H
#define EFFECTS_DATA_H

#define WACOM   0
#define TOUCH   1

//[DOCTYPE]
#define NOTEBOOK        0
#define PDF             1


#define NUM_EFFECTS     12
//[EFFECT]:
#define NULL_EFFECT       0 //null effect
#define TOOLBAR           1 //action: toolbar
#define WRITING           2 //action: writing
#define STROKE_PANEL      3 //tap on stroke panel, not useful on it's own
#define TEXT              4 //action: text button
#define ERASER_PANEL      5 //tap on eraser panel, not useful on it's own
#define SELECT            6 //action: select
#define UNDO              7 //action: undo button
#define REDO              8 //action: redo button
#define ERASER_ERASE      9 //action: eraser
#define ERASER_SELECT    10 //action: eraser-select
#define ERASER_ERASE_ALL 11 //action: eraser-eraser-all

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
  "null-effect",        //null effect        0
  "toolbar",            //TOOLBAR            1
  "writing",            //WRITING            2
  "stroke-panel",       //STROKE             3
  "text",               //TEXT               4
  "eraser-panel",       //ERASER             5
  "select",             //SELECT             6
  "undo",               //UNDO               7
  "redo",               //REDO               8
  "eraser-eraser",      //ERASER_ERASE       9
  "eraser-select",      //ERASER_SELECT     10
  "eraser-erase-all"    //ERASER_ERASE_ALL  11
};


#define T_RM1_RHY 670 //define RM1 toolbar column touchscreen location
#define T_RM1_RHX 530
#define T_RM1_LHY 780
#define T_RM1_LHX 520
#define T_RM2_RHX 60  //define RM2 toolbar column touchscreen location
#define T_RM2_RHY 50
#define T_RM2_LHX 1360
#define T_RM2_LHY 1820
//                                  [rmVersion][DOCTYPE][EFFECT][ORIENTATION][x/y]
static const int locationLookupTouch[2][2][NUM_EFFECTS][4][2] = {
  { //RM1
    { //NOTEBOOK
        //RHP              //RHL              //LHP              //LHL
      { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT       0
      { {T_RM1_RHX,  990}, { 720, T_RM1_RHY}, {T_RM1_LHX,  990}, { 720, T_RM1_LHY} }, //TOOLBAR           1
      { {T_RM1_RHX,  930}, { 670, T_RM1_RHY}, {T_RM1_LHX,  930}, { 670, T_RM1_LHY} }, //WRITING           2
      { {T_RM1_RHX,  870}, { 600, T_RM1_RHY}, {T_RM1_LHX,  870}, { 600, T_RM1_LHY} }, //STROKE            3
      { {T_RM1_RHX,  870}, { 600, T_RM1_RHY}, {T_RM1_LHX,  870}, { 600, T_RM1_LHY} }, //TEXT              4
      { {T_RM1_RHX,  800}, { 530, T_RM1_RHY}, {T_RM1_LHX,  800}, { 530, T_RM1_LHY} }, //ERASER_PANEL      5
      { {T_RM1_RHX,  720}, { 480, T_RM1_RHY}, {T_RM1_LHX,  720}, { 460, T_RM1_LHY} }, //SELECT            6
      { {T_RM1_RHX,  660}, { 400, T_RM1_RHY}, {T_RM1_LHX,  660}, { 400, T_RM1_LHY} }, //UNDO              7
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //REDO              8
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //ERASER_ERASE      9
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //ERASER_SELECT    10
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //ERASER_ERASE_ALL 11
    },
    { //PDF
        //RHP             //RHL              //LHP              //LHL
      { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT       0
      { {T_RM1_RHX,  990}, { 720, T_RM1_RHY}, {T_RM1_LHX,  990}, { 720, T_RM1_LHY} }, //TOOLBAR           1
      { {T_RM1_RHX,  930}, { 670, T_RM1_RHY}, {T_RM1_LHX,  930}, { 670, T_RM1_LHY} }, //WRITING           2
      { {T_RM1_RHX,  870}, { 600, T_RM1_RHY}, {T_RM1_LHX,  870}, { 600, T_RM1_LHY} }, //STROKE            3
      { {T_RM1_RHX,  870}, { 600, T_RM1_RHY}, {T_RM1_LHX,  870}, { 600, T_RM1_LHY} }, //TEXT              4
      { {T_RM1_RHX,  800}, { 530, T_RM1_RHY}, {T_RM1_LHX,  800}, { 530, T_RM1_LHY} }, //ERASER_PANEL      5
      { {T_RM1_RHX,  720}, { 480, T_RM1_RHY}, {T_RM1_LHX,  720}, { 460, T_RM1_LHY} }, //SELECT            6
      { {T_RM1_RHX,  660}, { 400, T_RM1_RHY}, {T_RM1_LHX,  660}, { 400, T_RM1_LHY} }, //UNDO              7
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //REDO              8
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //ERASER_ERASE      9
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //ERASER_SELECT    10
      { {T_RM1_RHX,  600}, { 330, T_RM1_RHY}, {T_RM1_LHX,  600}, { 340, T_RM1_LHY} }, //ERASER_ERASE_ALL 11
    },
  },
  { //RM2
    { //NOTEBOOK
        //RHP              //RHL              //LHP             //LHL
      { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT       0
      { {T_RM2_RHX, 1850}, {  70, T_RM2_RHY}, {T_RM2_LHX, 1810}, {  80, T_RM2_LHY} }, //TOOLBAR           1
      { {T_RM2_RHX, 1720}, { 150, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING           2
      { {T_RM2_RHX, 1620}, { 260, T_RM2_RHY}, {T_RM2_LHX, 1600}, { 270, T_RM2_LHY} }, //STROKE            3
      { {T_RM2_RHX, 1510}, { 360, T_RM2_RHY}, {T_RM2_LHX, 1500}, { 370, T_RM2_LHY} }, //TEXT              4
      { {T_RM2_RHX, 1410}, { 450, T_RM2_RHY}, {T_RM2_LHX, 1400}, { 480, T_RM2_LHY} }, //ERASER_PANEL      5
      { {T_RM2_RHX, 1300}, { 570, T_RM2_RHY}, {T_RM2_LHX, 1230}, { 590, T_RM2_LHY} }, //SELECT            6
      { {T_RM2_RHX, 1190}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1200}, { 690, T_RM2_LHY} }, //UNDO              7
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //REDO              8
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_ERASE      9
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_SELECT    10
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_ERASE_ALL 11
    },
    { //PDF
      //RHP              //RHL              //LHP             //LHL
      { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT       0
      { {T_RM2_RHX, 1850}, {  70, T_RM2_RHY}, {T_RM2_LHX, 1810}, {  80, T_RM2_LHY} }, //TOOLBAR           1
      { {T_RM2_RHX, 1720}, { 150, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING           2
      { {T_RM2_RHX, 1620}, { 260, T_RM2_RHY}, {T_RM2_LHX, 1600}, { 270, T_RM2_LHY} }, //STROKE            3
      { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //TEXT              4
      { {T_RM2_RHX, 1410}, { 450, T_RM2_RHY}, {T_RM2_LHX, 1400}, { 480, T_RM2_LHY} }, //ERASER_PANEL      5
      { {T_RM2_RHX, 1300}, { 570, T_RM2_RHY}, {T_RM2_LHX, 1230}, { 590, T_RM2_LHY} }, //SELECT            6
      { {T_RM2_RHX, 1190}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1200}, { 690, T_RM2_LHY} }, //UNDO              7
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //REDO              8
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_ERASE      9
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_SELECT    10
      { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1100}, { 790, T_RM2_LHY} }, //ERASER_ERASE_ALL 11
    }
  }
};


#define W_RM2_RHY 730 //define toolbar column wacom location
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
