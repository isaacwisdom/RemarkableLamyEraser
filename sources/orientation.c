#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orientation.h"

int get_open_file_from_uuid(char *uuid) {
  // returns  1 if success
  // returns  0 if no open file
  // returns -1 if failure
  char  buf[BUFSIZE] = "";
  char *command      = "PID=`pidof xochitl`; ls -l /proc/$PID/fd 2> /dev/null "
                       "| grep lock | egrep -v 'rm|xochitl/xochitl' "
                       "| egrep '[0-9a-f]{8}-([0-9a-f]{4}-){3}[0-9a-f]{12}' -o";
  FILE *cmd          = popen(command, "r");
  if (cmd == NULL) {
    printf("getOpenFileGUID: Failed to run command\n");
    return -1;
  }
  fgets(buf, BUFSIZE, cmd);
  pclose(cmd);

  if (strlen(buf) == 0) {
    return 0; // return 0 if no open file
  }
  strcpy(uuid, buf);
  uuid[strcspn(uuid, "\n")] = 0; // trim off new line
  return 1;                      // return 1 if open file
}

int check_conf(const char *path, const char *param, const char *param_true) {
  // returns 1 if param matches paramTrue
  // returns 0 if param doesn't match paramTrue
  // returns -1 if failure
  FILE   *fp;
  char   *line = NULL;
  size_t  len  = 0;
  ssize_t read;
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("checkConf: File %s doesn't exist\n", path);
    return -1;
  }
  // int i = 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    // printf("line %d: ", i++);
    // printf("%s", line);
    if (!strncmp(line, param, strlen(param))) {
      // printf("%s", line);
      int value = !strncmp(line, param_true, strlen(param_true));
      fclose(fp);
      return value;
    }
  }
  return -1;
}

int get_conf(const char *path, const char *param, char *return_string, int buffer_size) {
  // puts text immeadiatetly after param into return array passed in.
  // if param can't be found, returns -1, 1 on success
  if (!return_string || buffer_size < 1) {
    printf("getConf(): bad input\n");
    return -1; // bad input
  }
  FILE   *fp;
  char   *line = NULL;
  size_t  len  = 0;
  ssize_t read;
  fp = fopen(path, "r");
  if (fp == NULL) {
    printf("getConf: File %s doesn't exist\n", path);
    return -1;
  }
  // int i = 1;
  while ((read = getline(&line, &len, fp)) != -1) {
    // printf("line: %d ", i++);
    // printf("%s", line);
    if (!strncmp(line, param, strlen(param))) {
      // printf("%s", line);
      strncpy(return_string, line + strlen(param),
              buffer_size); // copy config line text after param
      return_string[buffer_size - 1] = '\0';
      fclose(fp);
      return 1; // successfully put value of param into returnString
    }
  }
  return -1;
}

toolbar_orientation get_toolbar_orientation() {
  // returns panelOrientation struct with params:
  // {openNotebook | 1 for open notebook, 0 for no open notebook
  //  orientation  | 0 for RHP, 1 for RHL, 2 for LHP, 3 for LHL
  //                 the three letter acronyms are defined macros}
  //  docType      | 0 for notebook, 1 for pdf

  // TODO: getOpenFileUUID is broken, so default some values for now.
  toolbar_orientation orientation = {1, 0, 0};
  return orientation;

  /* int rightHanded; */
  /* int portrait; */
  /* // get portrait or landscape */
  /* char UUID[BUFSIZE]; */
  /* if ( (orientation.openNotebook = getOpenFileUUID(UUID)) == 1) { */
  /*   char openFilePath[128] = "/home/root/.local/share/remarkable/xochitl/"; */
  /*   strcat(UUID, ".content"); */
  /*   strcat(openFilePath, UUID); */
  /*   // printf("%s\n", openFilePath); */
  /*   portrait = checkConf(openFilePath, "    \"orientation\"", "    \"orientation\": \"portrait\""); */
  /*   orientation.docType = checkConf(openFilePath, "    \"fileType\"", "    \"fileType\": \"pdf\""); */
  /*   } */
  /* else { */
  /*   return orientation; // other params will be -1 to indicate orientation and docType is N/A */
  /*   } */
  /* // get handedness */
  /* const char *confPath = "/home/root/.config/remarkable/xochitl.conf"; */
  /* rightHanded = checkConf(confPath, "RightHanded", "RightHanded=true"); */
  /* orientation.orientation = !portrait + (!rightHanded << 1); */
  /* //printf("Orientation: %d, docType:%d", orientation.orientation, orientation.docType); */
  /* return orientation; */
}

int get_rm_version() {
  const char *sysPath = "/sys/devices/soc0/machine";
  int         RM2     = check_conf(sysPath, "reMarkable", "reMarkable 2.0");
  return RM2 + 1; // returns 1 for RM1, 2 for RM2
}

int get_software_version(int software_version_array[4]) {
  // returns -1 if failed to get software versions, 1 on success.
  const char *sysPath = "/usr/share/remarkable/update.conf";
  char        software_version[15];
  char       *token;

  if (-1 == get_conf(sysPath, "REMARKABLE_RELEASE_VERSION=", software_version,
                     sizeof(software_version))) {
    return -1;
  } else {
    token = strtok(software_version, ".");
    for (int i = 0; i < 4; i++) {
      software_version_array[i] = atoi(token);
      token                   = strtok(NULL, ".");
    }
    return 1;
  }
}
