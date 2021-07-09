#ifndef ORIENTATION
#define ORIENTATION

/*
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
*/

#define BUFSIZE 48

struct panelOrientation {
  int openNotebook;
  int rightHanded;
  int portrait;
};

int getOpenFileUUID(char *UUID);
int checkConf(const char *path, const char *param, const char *paramTrue);
struct panelOrientation getPanelOrientation();


#endif
