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

//recognized words in configuration file.
static const char* triggers[NUM_TRIGGERS] = {
  "",                      //for alignment    0
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


int getTriggerConfig(const char *path, struct configuration* config);
void printConfig(struct configuration* config);

#endif
