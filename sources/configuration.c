#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <configuration.h>
#include <effects.h>
#include <effects_data.h>

int get_trigger_config(const char *path, struct configuration *config) {
  // returns 0 on success
  // returns 1 if error in configuration file
  //  returns -1 if configuration file doesn't exist
  FILE   *fp;
  char   *line;
  int     lineNum = 0;
  size_t  len     = 0;
  ssize_t read;
  char    cfg[24]  = "--";
  char    effect[24]   = "--";
  int     read_cfg = -1, read_effect = -1;

  fp = fopen(path, "r");
  if (fp == NULL) return -1;
  while ((read = getline(&line, &len, fp)) != -1) {
    lineNum++;
    if (line[0] == '#' || line[0] == '\n') {
      continue;
    }

    if (sscanf(line, "%s %s", cfg, effect) == 2) {
      printf("Line %2d: Read trigger <%s> with effect <%s>\n", lineNum, cfg, effect);
    } else {
      // printf("Line %2d: Read trigger <%s> has no effect.\n", lineNum, cfg);
      strcpy(effect, "null-effect");
    }

    for (int i = 0; i < NUM_TOTAL_CONFIGS; i++) {
      if (!strcmp(cfg, configs[i])) {
        read_cfg = i;
        break;
      }
    }
    for (int j = 0; j < NUM_EFFECTS; j++) {
      if (!strcmp(effect, EFFECTS[j])) {
        read_effect = j;
        break;
      }
    }
    if (read_cfg == -1) {
      printf("Invalid config key %s in line %d of configuration file at %s\n", cfg,
             lineNum, path);
      return 1;
    }
    if (read_effect == -1) {
      printf("Invalid effect %s in line %d of configuration file at %s\n", effect,
             lineNum, path);
      return 1;
    }

    switch (read_cfg) {
      case TRIGGER_CLICK_1: config->clickEffect[0]     = read_effect; break;
      case TRIGGER_CLICK_2: config->clickEffect[1]     = read_effect; break;
      case TRIGGER_CLICK_3: config->clickEffect[2]     = read_effect; break;
      case TRIGGER_CLICK_4: config->clickEffect[3]     = read_effect; break;
      case TRIGGER_CLICK_5: config->clickEffect[4]     = read_effect; break;
      case TRIGGER_HOLD_1:  config->holdEffect[0]      = read_effect; break;
      case TRIGGER_HOLD_2:  config->holdEffect[1]      = read_effect; break;
      case TRIGGER_HOLD_3:  config->holdEffect[2]      = read_effect; break;
      case TRIGGER_HOLD_4:  config->holdEffect[3]      = read_effect; break;
      case TRIGGER_HOLD_5:  config->holdEffect[4]      = read_effect; break;
      case TRIGGER_LCLICK:  config->longClickEffect[0] = read_effect; break;
      case ASSUME_TB_OPEN:  config->assumeTBOpen       = 1          ; break;
    }
    strcpy(cfg, "--");
    strcpy(effect, "--");
    read_cfg = -1;
    read_effect  = -1;
  }
  fclose(fp);
  return 0;
}

void print_config(struct configuration *config) {
  printf("click1Effect: %x\n", config->clickEffect[0]);
  printf("click2Effect: %x\n", config->clickEffect[1]);
  printf("click3Effect: %x\n", config->clickEffect[2]);
  printf("click4Effect: %x\n", config->clickEffect[3]);
  printf("click5Effect: %x\n", config->clickEffect[4]);
  printf("hold1Effect: %x\n", config->holdEffect[0]);
  printf("hold2Effect: %x\n", config->holdEffect[1]);
  printf("hold3Effect: %x\n", config->holdEffect[2]);
  printf("hold4Effect: %x\n", config->holdEffect[3]);
  printf("hold5Effect: %x\n", config->holdEffect[4]);
  printf("longClick1Effect: %x\n", config->longClickEffect[0]);
}
