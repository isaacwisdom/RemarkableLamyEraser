#ifndef ORIENTATION_H
#define ORIENTATION_H

#define BUFSIZE 48
#define RHP     0 // Right Hand, Portrait
#define RHL     1 // Right Hand, Landscape
#define LHP     2 // Left Hand,  Portrait
#define LHL     3 // Left Hand,  Landscape


typedef struct {
  int openNotebook;
  int orientation;
} toolbarOrientation;

int getOpenFileUUID(char *UUID);
toolbarOrientation getToolbarOrientation();
int getRmVersion();


#endif //ORIENTATION_H
