#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "orientation.h"

enum doc_type get_open_file_uuid(char *out_uuid) {
  char  buf[BUFSIZE] = "";
  char *command      = "ls -l /proc/*/fd 2>/dev/null " // lsof ersatz
                  "| grep pdf "                        // see if any PDF is open
                  "| egrep '[0-9a-f]{8}-([0-9a-f]{4}-){3}[0-9a-f]{12}' -o"; // UUID format
  FILE *cmd = popen(command, "r");
  if (cmd == NULL) {
    printf("get_open_file_uuid: Failed to run command\n");
    return -1;
  }
  fgets(buf, BUFSIZE, cmd);
  pclose(cmd);
  if (strlen(buf) == 0) {
    return notebook; // No open file = notebook
  }
  strcpy(out_uuid, buf);
  out_uuid[strcspn(out_uuid, "\n")] = 0; // Trim off new line
  return pdf;
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
  char                filename[BUFSIZE];
  toolbar_orientation orientation = {RHP, get_open_file_uuid(filename)};
  if (orientation.doc_type == pdf) {
    char open_file_path[128] = "/home/root/.local/share/remarkable/xochitl/";
    strcat(filename, ".content");
    strcat(open_file_path, filename);
    // Orientation
    int portrait = check_conf(open_file_path, "    \"orientation\"",
                              "    \"orientation\": \"portrait\"");
    // Handedness
    int right_handed        = check_conf("/home/root/.config/remarkable/xochitl.conf",
                                         "RightHanded", "RightHanded=true");
    orientation.orientation = !portrait + (!right_handed << 1);
    return orientation;
  } else {
    // XXX: Default to right-handed notebook for now.
    return orientation;
  }
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
      token                     = strtok(NULL, ".");
    }
    return 1;
  }
}
