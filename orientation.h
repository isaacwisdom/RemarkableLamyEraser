#ifndef ORIENTATION_H
#define ORIENTATION_H

#define BUFSIZE 48

typedef struct {
  int openNotebook;
  int rightHanded;
  int portrait;
} toolbarOrientation;

int getOpenFileUUID(char *UUID);
int checkConf(const char *path, const char *param, const char *paramTrue);
toolbarOrientation getToolbarOrientation();


#endif //ORIENTATION_H
