#ifndef ORIENTATION_H
#define ORIENTATION_H

#define BUFSIZE 48

enum orientation {
  RHP, // Right Hand, Portrait
  RHL, // Right Hand, Landscape
  LHP, // Left Hand,  Portrait
  LHL, // Left Hand,  Landscape
};

enum doc_type {
  notebook,
  pdf,
};

typedef struct {
  enum orientation orientation;
  enum doc_type    doc_type;
} toolbar_orientation;

int get_open_file_from_uuid(char *uuid);

int check_conf(const char *path, const char *param, const char *param_true);

int get_conf(const char *path, const char *param, char *return_string, int buf_size);

toolbar_orientation get_toolbar_orientation();

int get_rm_version();

int get_software_version();

#endif // ORIENTATION_H
