#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "actions.h"

#define PEN_DEVICE_RM2   "/dev/input/event1"
#define TOUCH_DEVICE_RM2 "/dev/input/event2"

int main(int argc, char *argv[]) {
  int mode = 0, doublePressAction = 0;
  struct input_event ev_pen;
  int fd_pen, fd_touch;

  char name[256] = "Unknown";

  printf("RemarkableLamyEraser 1.1\n");
  // check our input args
  for (int i = 1; i < argc; i++) {
    if (!strncmp(argv[i], "--toggle", 8)) {
      printf("MODE: TOGGLE\n");
      mode = TOGGLE_MODE;
    } else if (!strncmp(argv[i], "--press", 7)) {
      printf("MODE: PRESS AND HOLD\n");
      mode = PRESS_MODE;
    } else if (!strncmp(argv[i], "--double-press", 14)) {
      if (!strncmp(argv[i + 1], "undo", 4)) {
        printf("DOUBLE CLICK ACTION: UNDO\n");
        doublePressAction = UNDO;
        i++; // manually increment i so we skip interpretting the double press action arg
      } else if (!strncmp(argv[i + 1], "redo", 4)) {
        printf("DOUBLE CLICK ACTION: REDO\n");
        doublePressAction = REDO;
        i++; // manually increment i so we skip interpretting the double press action arg
      } else {
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
    mode = PRESS_MODE;
  }
  if (!doublePressAction) {
    printf("No double click action specified! Using default.\nDOUBLE CLICK "
           "ACTION: UNDO\n");
    doublePressAction = UNDO;
  }

  /* Open Device: Pen */
  fd_pen = open(PEN_DEVICE_RM2, O_RDWR);
  if (fd_pen == -1) {
    fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE_RM2);
    exit(EXIT_FAILURE);
  }
  /* Open Device: Touch */
  fd_touch = open(TOUCH_DEVICE_RM2, O_WRONLY);
  if (fd_touch == -1) {
    fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE_RM2);
    exit(EXIT_FAILURE);
  }

  /* Print Device Name */
  ioctl(fd_pen, EVIOCGNAME(sizeof(name)), name);
  printf("Using Devices:\n");
  printf("1. device file = %s\n", PEN_DEVICE_RM2);
  printf("   device name = %s\n", name);
  ioctl(fd_touch, EVIOCGNAME(sizeof(name)), name);
  printf("2. device file = %s\n", TOUCH_DEVICE_RM2);
  printf("   device name = %s\n", name);

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
    case TOGGLE_MODE:
      toggleMode(ev_pen, fd_pen);
      break;
    case PRESS_MODE:
      pressMode(ev_pen, fd_pen);
      break;
    default:
      printf("Somehow a mode wasn't set? Exiting...\n");
      exit(EXIT_FAILURE);
    }
  }
  return EXIT_SUCCESS;
}
