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
  char    trigger[24]  = "--";
  char    effect[24]   = "--";
  int     read_trigger = -1, read_effect = -1;

  fp = fopen(path, "r");
  if (fp == NULL) return -1;
  while ((read = getline(&line, &len, fp)) != -1) {
    lineNum++;
    if (line[0] == '#' || line[0] == '\n') {
      continue;
    }

    if (sscanf(line, "%s %s", trigger, effect) == 2) {
      printf("Line %2d: Read trigger <%s> with effect <%s>\n", lineNum, trigger, effect);
    } else {
      // printf("Line %2d: Read trigger <%s> has no effect.\n", lineNum, trigger);
      strcpy(effect, "null-effect");
    }

    for (int i = 0; i < NUM_TRIGGERS; i++) {
      if (!strcmp(trigger, triggers[i])) {
        read_trigger = i;
        break;
      }
    }
    for (int j = 0; j < NUM_EFFECTS; j++) {
      if (!strcmp(effect, EFFECTS[j])) {
        read_effect = j;
        break;
      }
    }
    if (read_trigger == -1) {
      printf("Invalid trigger %s in line %d of configuration file at %s\n", trigger,
             lineNum, path);
      return 1;
    }
    if (read_effect == -1) {
      printf("Invalid effect %s in line %d of configuration file at %s\n", effect,
             lineNum, path);
      return 1;
    }

    switch (read_trigger) {
      case TRIGGER_CLICK_1: config->click1Effect = read_effect; break;
      case TRIGGER_CLICK_2: config->click2Effect = read_effect; break;
      case TRIGGER_CLICK_3: config->click3Effect = read_effect; break;
      case TRIGGER_CLICK_4: config->click4Effect = read_effect; break;
      case TRIGGER_CLICK_5: config->click5Effect = read_effect; break;
      case TRIGGER_HOLD_1:  config->hold1Effect  = read_effect; break;
      case TRIGGER_HOLD_2:  config->hold2Effect  = read_effect; break;
      case TRIGGER_HOLD_3:  config->hold3Effect  = read_effect; break;
      case TRIGGER_HOLD_4:  config->hold4Effect  = read_effect; break;
      case TRIGGER_HOLD_5:  config->hold5Effect  = read_effect; break;
    }
    strcpy(trigger, "--");
    strcpy(effect, "--");
    read_trigger = -1;
    read_effect  = -1;
  }
  fclose(fp);
  return 0;
}

void print_config(struct configuration *config) {
  printf("click1Effect: %x\n", config->click1Effect);
  printf("click2Effect: %x\n", config->click2Effect);
  printf("click3Effect: %x\n", config->click3Effect);
  printf("click4Effect: %x\n", config->click4Effect);
  printf("click5Effect: %x\n", config->click5Effect);
  printf("hold1Effect: %x\n", config->hold1Effect);
  printf("hold2Effect: %x\n", config->hold2Effect);
  printf("hold3Effect: %x\n", config->hold3Effect);
  printf("hold4Effect: %x\n", config->hold4Effect);
  printf("hold5Effect: %x\n", config->hold5Effect);
}
