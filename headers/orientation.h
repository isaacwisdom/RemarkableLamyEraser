#ifndef ORIENTATION_H
#define ORIENTATION_H

#define BUFSIZE 48
#define RHP 0 // Right Hand, Portrait
#define RHL 1 // Right Hand, Landscape
#define LHP 2 // Left Hand,  Portrait
#define LHL 3 // Left Hand,  Landscape

typedef struct {
  int openNotebook;
  int orientation;
  int docType;
} toolbar_orientation;

int get_open_file_from_uuid(char *uuid);

int check_conf(const char *path, const char *param, const char *param_true);

int get_conf(const char *path, const char *param, char *return_string, int buf_size);

toolbar_orientation get_toolbar_orientation();

int get_rm_version();

int get_software_version();

#endif // ORIENTATION_H
