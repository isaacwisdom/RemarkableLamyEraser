#ifndef EFFECTS_DATA_H
#define EFFECTS_DATA_H

#define WACOM   0
#define TOUCH   1

//[DOCTYPE]
#define NOTEBOOK 0
#define PDF      1

enum effects {
  NULL_EFFECT,              // Null effect
  TOOLBAR,                  // action: toolbar
  WRITING1,                 // action: writing 1
  WRITING2,                 // action: writing 2
  TEXT,                     // action: text button
  ERASER_PANEL,             // tap on eraser panel, not useful on it's own
  SELECT,                   // action: select
  UNDO,                     // action: undo button
  REDO,                     // action: redo button
  ERASER_ERASE,             // action: eraser
  ERASER_SELECTION,         // action: eraser-select
  ERASER_ERASE_ALL,         // action: eraser-eraser-all
  WRITING_FINELINER,        // action: writing-fineliner
  WRITING_CALLIGRAPHY,      // action: writing-calligraphy
  WRITING_BLACK,            // action: writing-black
  WRITING_GREY,             // action: writing-grey
  WRITING_HL,               // action: writing-hl
  ONE_OFF_ERASER_SELECTION, // action: one-off-eraser-selection
  ONE_OFF_ERASER,           // action: one-off-eraser
  ONE_OFF_HL,               // action: one-off-hl-selection
  NUM_EFFECTS,              // Total number of effects
};

enum effect_type {
  temp_on,
  toggle,
  temp_off,
};

//recognized words in config file
static const char* EFFECTS[NUM_EFFECTS] = {
  "null-effect",              //null effect               0
  "toolbar",                  //TOOLBAR                   1
  "writing1",                 //WRITING1                  2
  "writing2",                 //WRITING2                  3
  "text",                     //TEXT                      4
  "eraser-panel",             //ERASER                    5
  "select",                   //SELECT                    6
  "undo",                     //UNDO                      7
  "redo",                     //REDO                      8
  "eraser-erase",             //ERASER_ERASE              9
  "eraser-selection",         //ERASER_SELECT            10
  "eraser-erase-all",         //ERASER_ERASE_ALL         11
  "writing-fineliner",        //WRITING_FINELINER        12
  "writing-calligraphy",      //WRITING_CALLIGRAPHY      13
  "writing-black",            //WRITING_BLACK            14
  "writing-grey",             //WRITING_GREY             15
  "writing-hl",               //WRITING_HL               16
  "one-off-eraser-selection", //ONE_OFF_ERASER_SELECTION 17
  "one-off-eraser",           //ONE_OFF_ERASER           18
  "one-off-hl",               //ONE_OFF_HL               19
};

#define T_RM2_RHX 60  // Define RM2 toolbar column touchscreen location
#define T_RM2_RHY 50
#define T_RM2_LHX 1360
#define T_RM2_LHY 1820

static const int LOCATION_LOOKUP_TOUCH[2][NUM_EFFECTS][4][2] = {
  {//NOTEBOOK
   // RHP                RHL                LHP                LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT          0
    { {T_RM2_RHX, 1850}, {  30, T_RM2_RHY}, {T_RM2_LHX, 1810}, {  80, T_RM2_LHY} }, //TOOLBAR              1
    { {T_RM2_RHX, 1720}, { 150, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING 1            2
    { {T_RM2_RHX, 1620}, { 250, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING 2            3
    { {T_RM2_RHX, 1510}, { 350, T_RM2_RHY}, {T_RM2_LHX, 1600}, { 270, T_RM2_LHY} }, //TEXT                 4
    { {T_RM2_RHX, 1410}, { 460, T_RM2_RHY}, {T_RM2_LHX, 1510}, { 360, T_RM2_LHY} }, //ERASER_PANEL         5
    { {T_RM2_RHX, 1300}, { 570, T_RM2_RHY}, {T_RM2_LHX, 1410}, { 440, T_RM2_LHY} }, //SELECT               6
    { {T_RM2_RHX, 1190}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1300}, { 550, T_RM2_LHY} }, //UNDO                 7
    { {T_RM2_RHX, 1090}, { 770, T_RM2_RHY}, {T_RM2_LHX, 1190}, { 670, T_RM2_LHY} }, //REDO                 8
    { {      180, 1340}, { 530,       160}, {     1000, 1440}, { 430,      1450} }, //ERASER_ERASE         9
    { {      250, 1480}, { 540,       300}, {     1140, 1440}, { 430,      1580} }, //ERASER_SELECTION    10
    { {      200, 1380}, { 670,       160}, {     1050, 1280}, { 670,      1480} }, //ERASER_ERASE_ALL    11
    { {      340, 1630}, { 230,       350}, {     1010, 1640}, { 230,      1480} }, //WRITING_FINELINER   12
    { {      470, 1540}, { 330,       490}, {     1220, 1530}, { 330,      1680} }, //WRITING_CALLIGRAPHY 13
    { {      200, 1050}, { 840,       200}, {      900, 1120}, { 730,      1380} }, //WRITING_BLACK       14
    { {      330, 1050}, { 840,       340}, {     1020, 1140}, { 730,      1480} }, //WRITING_GREY        15
    { {      470, 1630}, { 230,       490}, {      900, 1530}, { 330,      1370} }, //WRITING_HL          16
    {}, // ONE_OFF_ERASER_SELECTION (same as ERASER_SELECTION) 17
    {}, // ONE_OFF_ERASER           (same as ERASER)           18
    {}, // ONE_OFF_HL (same as WRITING_HL)                     19
  },
  {//PDF
   // RHP                RHL                LHP                LHL
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //NULL_EFFECT          0
    { {T_RM2_RHX, 1850}, {  60, T_RM2_RHY}, {T_RM2_LHX, 1810}, {  80, T_RM2_LHY} }, //TOOLBAR              1
    { {T_RM2_RHX, 1720}, { 150, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING 1            2
    { {T_RM2_RHX, 1620}, { 250, T_RM2_RHY}, {T_RM2_LHX, 1700}, { 160, T_RM2_LHY} }, //WRITING 2            3
    { {        0,    0}, {   0,         0}, {        0,    0}, {   0,         0} }, //TEXT                 4
    { {T_RM2_RHX, 1510}, { 350, T_RM2_RHY}, {T_RM2_LHX, 1600}, { 270, T_RM2_LHY} }, //ERASER_PANEL         5
    { {T_RM2_RHX, 1410}, { 460, T_RM2_RHY}, {T_RM2_LHX, 1510}, { 360, T_RM2_LHY} }, //SELECT               6
    { {T_RM2_RHX, 1300}, { 570, T_RM2_RHY}, {T_RM2_LHX, 1410}, { 470, T_RM2_LHY} }, //UNDO                 7
    { {T_RM2_RHX, 1190}, { 670, T_RM2_RHY}, {T_RM2_LHX, 1300}, { 580, T_RM2_LHY} }, //REDO                 8
    { {      180, 1440}, { 410,       160}, {     1000, 1530}, { 340,      1450} }, //ERASER_ERASE         9
    { {      250, 1470}, { 410,       300}, {     1140, 1530}, { 340,      1580} }, //ERASER_SELECTION    10
    { {      200, 1300}, { 580,       160}, {     1050, 1400}, { 580,      1480} }, //ERASER_ERASE_ALL    11
    { {      340, 1630}, { 230,       350}, {     1010, 1640}, { 230,      1480} }, //WRITING_FINELINER   12
    { {      470, 1540}, { 330,       490}, {     1220, 1530}, { 330,      1680} }, //WRITING_CALLIGRAPHY 13
    { {      200, 1050}, { 840,       200}, {      900, 1120}, { 730,      1380} }, //WRITING_BLACK       14
    { {      330, 1050}, { 840,       340}, {     1020, 1140}, { 730,      1480} }, //WRITING_GREY        15
    { {      470, 1630}, { 330,       490}, {      900, 1530}, { 330,      1370} }, //WRITING_HL          16
    {}, // ONE_OFF_ERASER_SELECTION (same as ERASER_SELECTION) 17
    {}, // ONE_OFF_ERASER           (same as ERASER)           18
    {}, // ONE_OFF_HL (same as WRITING_HL)                     19
  }
};

#endif //EFFECTS_DATA_H
