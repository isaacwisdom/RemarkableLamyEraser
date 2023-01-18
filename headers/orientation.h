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
  int isPDF;
} toolbarOrientation;

int getOpenFileUUID(char* UUID);
int checkConf(const char *path, const char *param, const char *paramTrue);
int getConf(const char *path, const char *param, char* returnString, int bufferSize);
toolbarOrientation getToolbarOrientation();
int getRmVersion();
int getSoftwareVersion();


#endif //ORIENTATION_H
