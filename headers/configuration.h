#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct configuration {
  int clickEffect[5];
  int longClickEffect[5];
  int holdEffect[5];
  int assumeTBOpen;
};

#define NUM_TRIGGERS  12
//triggers:
#define TRIGGER_CLICK_1    1
#define TRIGGER_CLICK_2    2
#define TRIGGER_CLICK_3    3
#define TRIGGER_CLICK_4    4
#define TRIGGER_CLICK_5    5
#define TRIGGER_HOLD_1     6
#define TRIGGER_HOLD_2     7
#define TRIGGER_HOLD_3     8
#define TRIGGER_HOLD_4     9
#define TRIGGER_HOLD_5     10
#define TRIGGER_LCLICK     11

#define NUM_FLAGS          1

//non-trigger config values
#define ASSUME_TB_OPEN     12

#define NUM_TOTAL_CONFIGS NUM_TRIGGERS + NUM_FLAGS

// Recognized words in configuration file.
static const char *configs[NUM_TOTAL_CONFIGS] = {
    "",                     // for alignment    0
    "click",                // TRIGGER_CLICK_1  1
    "double-click",         // TRIGGER_CLICK_2  2
    "triple-click",         // TRIGGER_CLICK_3  3
    "quadruple-click",      // TRIGGER_CLICK_4  4
    "quintuple-click",      // TRIGGER_CLICK_5  5
    "press&hold",           // TRIGGER_PRESS_1  6
    "double-press&hold",    // TRIGGER_PRESS_2  7
    "triple-press&hold",    // TRIGGER_PRESS_3  8
    "quadruple-press&hold", // TRIGGER_PRESS_4  9
    "quintuple-press&hold", // TRIGGER_PRESS_5  10
    "long-click",           // TRIGGER_LCLICK_1 11
    "assume-toolbar-open",  // ASSUME_TB_OPEN   12
};

int get_trigger_config(const char *path, struct configuration *config);

void print_config(struct configuration *config);

#endif
