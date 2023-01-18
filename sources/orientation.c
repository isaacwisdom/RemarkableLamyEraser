#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "orientation.h"


int getOpenFileUUID(char *UUID) {
  //returns  1 if success
  //returns  0 if no open file
  //returns -1 if failure
  char buf[BUFSIZE] = "";
  char *command = "PID=`pidof xochitl`; ls -l /proc/$PID/fd 2> /dev/null "
                  "| grep lock | egrep -v 'rm|xochitl/xochitl' "
                  "| egrep '[0-9a-f]{8}-([0-9a-f]{4}-){3}[0-9a-f]{12}' -o";
  FILE *cmd = popen(command, "r");
  if (cmd == NULL) {
    printf("getOpenFileGUID: Failed to run command\n");
    return -1;
  }
  fgets(buf, BUFSIZE, cmd);
  pclose(cmd);

  if (strlen(buf) == 0) {
    return 0; // return 0 if no open file
  }
  strcpy(UUID, buf);
  UUID[strcspn(UUID, "\n")] = 0; // trim off new line
  return 1;                      // return 1 if open file
}

int checkConf(const char *path, const char *param, const char *paramTrue) {
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
  //int i = 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    //printf("line %d: ", i++);
    //printf("%s", line);
    if (!strncmp(line, param, strlen(param))) {
      // printf("%s", line);
      int value = !strncmp(line, paramTrue, strlen(paramTrue));
      fclose(fp);
      return value;
    }
  }
  return -1;
}

int getConf(const char *path, const char *param, char *returnString, int bufferSize) {
    //puts text immeadiatetly after param into return array passed in.
    //if param can't be found, returns -1, 1 on success
    if(!returnString || bufferSize<1) {
        printf("getConf(): bad input\n");
        return -1; //bad input
      }
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(path, "r");
    if (fp == NULL) {
      printf("getConf: File %s doesn't exist\n", path);
      return -1;
    }
    //int i = 1;
    while ((read = getline(&line, &len, fp)) != -1) {
      //printf("line: %d ", i++);
      //printf("%s", line);
      if (!strncmp(line, param, strlen(param))) {
        //printf("%s", line);
        strncpy(returnString, line + strlen(param), bufferSize); //copy config line text after param
        returnString[bufferSize-1] = '\0';
        fclose(fp);
        return 1; //successfully put value of param into returnString
      }
    }
   return -1;
  }

toolbarOrientation getToolbarOrientation() {
  //returns panelOrientation struct with params:
  //{openNotebook | 1 for open notebook, 0 for no open notebook
  // orientation  | 0 for RHP, 1 for RHL, 2 for LHP, 3 for LHL
  //                the three letter acronyms are defined macros}
  // isPDF        | isPDF
  toolbarOrientation orientation = {-1,-1,-1};
  int rightHanded;
  int portrait;
  int isPDF;

  // get portrait or landscape
  char UUID[BUFSIZE];

  if ( (orientation.openNotebook = getOpenFileUUID(UUID)) == 1) {
    char openFilePath[128] = "/home/root/.local/share/remarkable/xochitl/";
    strcat(UUID, ".content");
    strcat(openFilePath, UUID);
    // printf("%s\n", openFilePath);
    portrait = checkConf(openFilePath, "    \"orientation\"", "    \"orientation\": \"portrait\"");
    isPDF = checkConf(openFilePath, "    \"fileType\"", "    \"fileType\": \"pdf\"");
    }
  else {
    return orientation; // other param will be -1 to indicate orientation is N/A
    }

  // get handedness
  const char *confPath = "/home/root/.config/remarkable/xochitl.conf";
  rightHanded = checkConf(confPath, "RightHanded", "RightHanded=true");
  orientation.orientation = !portrait + (!rightHanded << 1);


  printf("Orientation: %d, isPDF:%d", orientation.orientation, isPDF);
  return orientation;
}

int getRmVersion() {
  const char *sysPath = "/sys/devices/soc0/machine";
  int RM2 = checkConf(sysPath, "reMarkable", "reMarkable 2.0");
  return RM2 + 1; //returns 1 for RM1, 2 for RM2
}

int getSoftwareVersion (int softwareVersionArray[4]) {
  //returns -1 if failed to get software versions, 1 on success.
  const char *sysPath = "/home/root/.config/remarkable/xochitl.conf";
  char softwareVersion[15];
  char *token;

  if ( getConf(sysPath, "PreviousVersion=", softwareVersion, sizeof(softwareVersion)) == -1)
      return -1;
  //printf("string from conf file: %s\n", softwareVersion),

  token = strtok(softwareVersion, ".");
  for (int i=0; i<4; i++) {
      softwareVersionArray[i] = atoi(token);
      token = strtok(NULL, ".");
    }
  //printf("Version: %d.%d.%d.%d\n", softwareVersionArray[0], softwareVersionArray[1], softwareVersionArray[2], softwareVersionArray[3]);
  return 1;
}
