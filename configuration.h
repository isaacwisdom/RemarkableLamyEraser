#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct configuration {
  int click1Effect;
  int click2Effect;
  int click3Effect;
  int click4Effect;
  int click5Effect;
  int hold1Effect;
  int hold2Effect;
  int hold3Effect;
  int hold4Effect;
  int hold5Effect;
} configuration;



#define NUM_TRIGGERS  11
//triggers:
#define TRIGGER_CLICK_1   1
#define TRIGGER_CLICK_2   2
#define TRIGGER_CLICK_3   3
#define TRIGGER_CLICK_4   4
#define TRIGGER_CLICK_5   5
#define TRIGGER_HOLD_1    6
#define TRIGGER_HOLD_2    7
#define TRIGGER_HOLD_3    8
#define TRIGGER_HOLD_4    9
#define TRIGGER_HOLD_5    10

int configurationArray[NUM_TRIGGERS];

#define NULL_EFFECT       0


#define NUM_EFFECTS   9
//recognized words in configuration file. make sure these stay up to date with actions.h!
static const char *triggers[NUM_TRIGGERS] = {
  "alignment",             //for alignment, value is 0
  "click",                 //TRIGGER_CLICK_1  1
  "double-click",          //TRIGGER_CLICK_2  2
  "triple-click",          //TRIGGER_CLICK_3  3
  "quadruple-click",       //TRIGGER_CLICK_4  4
  "quintuple-click",       //TRIGGER_CLICK_5  5
  "press&hold",            //TRIGGER_PRESS_1  6
  "double-press&hold",     //TRIGGER_PRESS_2  7
  "triple-press&hold",     //TRIGGER_PRESS_3  8
  "quadruple-press&hold",  //TRIGGER_PRESS_4  9
  "quintuple-press&hold",  //TRIGGER_PRESS_5  10
};

static const char *effects[NUM_EFFECTS] = {
  "null",               //null effect      0
  "toolbar",            //TOOLBAR          1
  "writing",            //WRITING          2
  "ERASER_INTERNAL",    //ERASER           3
  "erase",              //ERASER_ERASE     4
  "erase-selection",    //ERASE_SELECTION  5
  "select",             //SELECT           6
  "undo",               //UNDO             7
  "redo",               //REDO             8
};


int getTriggerConfig(const char *path, struct configuration* config);
void printConfig(struct configuration* config);

#endif
