#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <configuration.h>

int checkConfig(const char *path, const char *param, const char *paramTrue) {
  //returns 1 if param matches paramTrue
  //returns 0 if param doesn't match paramTrue
  //returns -1 if failure
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("checkConf: File %s doesn't exist\n", path);
    return -1;
  }
  while ((read = getline(&line, &len, fp)) != -1) {
    // int i = 1;
    // printf("line: %d", i++);
    // printf("%s", line);
    if (!strncmp(line, param, strlen(param))) {
      // printf("%s", line);
      int value = !strncmp(line, paramTrue, strlen(paramTrue));
      fclose(fp);
      return value;
    }
  }
  return -1;
}


int getForceRM1Style(const char *path) {
  return checkConfig(path, "forceRM1", "forceRM1=true");
}
int getTriggerConfig(const char *path, struct configuration *config) {
  //returns 0 on success
  //returns 1 if error in configuration file
  // returns -1 if configuration file doesn't exist
  FILE *fp;
  char* line;
  int lineNum = 0;
  size_t len =0;
  ssize_t read;
  char trigger[24] = "--";
  char effect[24] = "--";
  int readTrigger = -1, readEffect = -1;

  fp = fopen(path, "r");
  if (fp == NULL)
    return -1;
  while ((read = getline(&line, &len, fp)) != -1) {
      lineNum++;
      if(!strncmp(line,"#DEBUG SETTINGS", 14)) {
          fclose(fp);
          return 0;
        }
      else if (line[0] == '#' || line[0] == '\n') {
          continue;
        }


      if ( (sscanf(line, "%s %s", trigger, effect) == 2) ) {
         printf("Line %2d: Read trigger <%s> with effect <%s>\n", lineNum, trigger, effect);
        }
      else {
        //printf("Line %2d: Read trigger <%s> has no effect.\n", lineNum, trigger);
        strcpy(effect, "null");
        }

      for (int i = 0; i < NUM_TRIGGERS; i++){
          if (!strcmp(trigger, triggers[i])) {
              readTrigger = i;
              break;
            }
        }
      for (int j = 0; j < NUM_EFFECTS; j++) {
        if (!strcmp(effect, effects[j])) {
            readEffect = j;
            break;
          }
        }
      if (readTrigger == -1) {
          printf("Invalid trigger %s in line %d of configuration file at %s\n", trigger, lineNum, path);
          return 1;
        }
      if (readEffect == -1) {
          printf("Invalid effect %s in line %d of configuration file at %s\n", effect, lineNum, path);
          return 1;
        }

      switch(readTrigger) {
        case TRIGGER_CLICK_1 :
          config->click1Effect = readEffect;
          break;
        case TRIGGER_CLICK_2 :
          config->click2Effect = readEffect;
          break;
        case TRIGGER_CLICK_3 :
          config->click3Effect = readEffect;
          break;
        case TRIGGER_CLICK_4 :
          config->click4Effect = readEffect;
          break;
        case TRIGGER_CLICK_5 :
          config->click5Effect = readEffect;
          break;
        case TRIGGER_HOLD_1 :
          config->hold1Effect = readEffect;
          break;
        case TRIGGER_HOLD_2 :
          config->hold2Effect = readEffect;
          break;
        case TRIGGER_HOLD_3 :
          config->hold3Effect = readEffect;
          break;
        case TRIGGER_HOLD_4 :
          config->hold4Effect = readEffect;
          break;
        case TRIGGER_HOLD_5 :
          config->hold5Effect = readEffect;
          break;
      }
      strcpy(trigger, "--");
      strcpy(effect, "--");
      readTrigger = -1;
      readEffect = -1;
  }
  fclose(fp);
  return 0;
}
void printConfig(struct configuration *config) {
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
