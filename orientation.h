#ifndef ORIENTATION_H
#define ORIENTATION_H

#define BUFSIZE 48

typedef struct {
  int openNotebook;
  int rightHanded;
  int portrait;
} toolbarOrientation;

int getOpenFileUUID(char *UUID);
toolbarOrientation getToolbarOrientation();
int getRmVersion();


#endif //ORIENTATION_H
