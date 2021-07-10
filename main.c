#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "actions.h"
#include "orientation.h"

#define PEN_DEVICE_RM2   "/dev/input/event1"
#define TOUCH_DEVICE_RM2 "/dev/input/event2"
#define PEN_DEVICE_RM1   "/dev/input/event0"
#define TOUCH_DEVICE_RM1 "/dev/input/event1"

int main(int argc, char *argv[]) {
  printf("RemarkableLamyEraser 1.1\n");
  printf("----------------------------------\n");

  int mode = 0, doublePressAction = 0;
  struct input_event ev_pen;
  int fd_pen, fd_touch;
  char name[256] = "Unknown";
  int rmVersion = getRmVersion();

  /* Open Input Devices */
  if (rmVersion == 2) {
      fd_pen = open(PEN_DEVICE_RM2, O_RDWR);
      fd_touch = open(TOUCH_DEVICE_RM2, O_WRONLY);
      if (fd_pen == -1) {
          fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE_RM2);
          exit(EXIT_FAILURE);
        }
      if (fd_pen == -1) {
          fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE_RM2);
          exit(EXIT_FAILURE);
        }
    }
  else {
      fd_pen = open(PEN_DEVICE_RM1, O_RDWR);
      fd_touch = open(TOUCH_DEVICE_RM1, O_WRONLY);
      if (fd_pen == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE_RM1);
        exit(EXIT_FAILURE);
      }
      if (fd_touch == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE_RM1);
        exit(EXIT_FAILURE);
    }
  }

  /* Print Device Names */
  printf("Detected ReMarkable %d. ", rmVersion);
  ioctl(fd_pen, EVIOCGNAME(sizeof(name)), name);
  printf("Using Devices:\n");
  printf("1. device file = %s\n", PEN_DEVICE_RM2);
  printf("   device name = %s\n", name);
  ioctl(fd_touch, EVIOCGNAME(sizeof(name)), name);
  printf("2. device file = %s\n", TOUCH_DEVICE_RM2);
  printf("   device name = %s\n", name);
  printf("----------------------------------\n");


  // check our input args
  for (int i = 1; i < argc; i++) {
    if (!strncmp(argv[i], "--force-RM1", 11)) {
      printf("Debug: FORCING RM1-STYLE ACTIONS\n");
      rmVersion = 1;
      }
    else if (!strncmp(argv[i], "--toggle", 8)) {
      printf("MODE: TOGGLE\n");
      mode = (rmVersion == 2) ? TOGGLE_MODE_RM2 : TOGGLE_MODE_RM1;
    }
    else if (!strncmp(argv[i], "--press", 7)) {
      printf("MODE: PRESS AND HOLD\n");
      mode = (rmVersion == 2) ? PRESS_MODE_RM2 : PRESS_MODE_RM1;
    }
    else if (!strncmp(argv[i], "--double-press", 14)) {
      if (!strncmp(argv[i + 1], "undo", 4)) {
        printf("DOUBLE CLICK ACTION: UNDO\n");
        doublePressAction = UNDO;
        i++; // manually increment i so we skip interpretting the double press action arg
      }
      else if (!strncmp(argv[i + 1], "redo", 4)) {
        printf("DOUBLE CLICK ACTION: REDO\n");
        doublePressAction = REDO;
        i++; // manually increment i so we skip interpretting the double press action arg
      }
      else {
        printf("Unknown double press action <%s>. Using default.\n", argv[i + 1]);
        printf("DOUBLE CLICK ACTION: UNDO\n");
        doublePressAction = UNDO;
      }
    } else {
      printf("Unknown argument %s\nExiting...\n", argv[i]);
      exit(EXIT_FAILURE);
    }
  }
  if (!mode) {
    printf("No mode specified! Using default.\nMODE: PRESS AND HOLD\n");
    mode = (rmVersion == 2) ? PRESS_MODE_RM2 : PRESS_MODE_RM1;
  }
  if (!doublePressAction) {
    printf("No double click action specified! Using default.\nDOUBLE CLICK "
           "ACTION: UNDO\n");
    doublePressAction = UNDO;
  }

  for (;;) {
    const size_t ev_pen_size = sizeof(struct input_event);
    read(fd_pen, &ev_pen, ev_pen_size); // note: read pauses until there is data

    if (doublePressHandler(ev_pen)) {
      switch (doublePressAction) {
      case UNDO:
        printf("writing undo\n");
        actionUndo(fd_touch);
        break;
      case REDO:
        printf("writing redo\n");
        actionRedo(fd_touch);
        break;
      }
    }

    switch (mode) {
    case TOGGLE_MODE_RM2:
      toggleMode(ev_pen, fd_pen);
      break;
    case PRESS_MODE_RM2:
      pressMode(ev_pen, fd_pen);
      break;
    case TOGGLE_MODE_RM1:
      toggleModeRM1(ev_pen, fd_touch);
      break;
    case PRESS_MODE_RM1:
      pressModeRM1(ev_pen, fd_touch);
      break;
    default:
      printf("Somehow a mode wasn't set? Exiting...\n");
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
