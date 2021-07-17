#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "triggers.h"
#include "actions.h"
#include "orientation.h"

#define PEN_DEVICE_RM2   "/dev/input/event1"
#define TOUCH_DEVICE_RM2 "/dev/input/event2"
#define PEN_DEVICE_RM1   "/dev/input/event0"
#define TOUCH_DEVICE_RM1 "/dev/input/event1"

int main(int argc, char *argv[]) {
  printf("RemarkableLamyEraser 1.1\n");
  printf("----------------------------------\n");

  int forceRM1 = 0, mode = 0, doubleClickAction = 0;
  int trigger;
  struct input_event ev_pen;
  const size_t input_event_size = sizeof(struct input_event);
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
    if (!strncmp(argv[i], "--press", 7)) {
      printf("MODE: PRESS AND HOLD\n");
      mode = PRESS_MODE;
    }
    else if (!strncmp(argv[i], "--toggle", 8)) {
      printf("MODE: TOGGLE\n");
      mode = TOGGLE_MODE;
    }
    else if (!strncmp(argv[i], "--double-press", 14)) {
      if (!strncmp(argv[i + 1], "undo", 4)) {
        printf("DOUBLE CLICK ACTION: UNDO\n");
        doubleClickAction = UNDO;
        i++; // manually increment i so we skip interpretting the double press action arg
      }
      else if (!strncmp(argv[i + 1], "redo", 4)) {
        printf("DOUBLE CLICK ACTION: REDO\n");
        doubleClickAction = REDO;
        i++; // manually increment i so we skip interpretting the double press action arg
      }
    else if (!strncmp(argv[i + 1], "select", 6)) {
        printf("DOUBLE CLICK ACTION: SELECT TOOL\n");
        doubleClickAction = SELECT;
        i++; // manually increment i so we skip interpretting the double press action arg
      }
      else {
        printf("Unknown double press action <%s>. Using default.\n", argv[i + 1]);
        printf("DOUBLE CLICK ACTION: UNDO\n");
        doubleClickAction = UNDO;
      }
    }
    else if (!strncmp(argv[i], "--force-RM1", 11)) {
      printf("Debug: FORCING RM1-STYLE ACTIONS\n");
      forceRM1 = 1;
      }
    else {
      printf("Unknown argument %s\nExiting...\n", argv[i]);
      exit(EXIT_FAILURE);
    }
  }

  //select appropraite mode based on RM version
  switch (mode) {
    case 0: //unset
      printf("No mode specified! Using default.\nMODE: PRESS AND HOLD\n");
      /* FALL-THROUGH */
    case PRESS_MODE:
      mode = (rmVersion == 2 && !forceRM1) ? PRESS_MODE_RM2 : PRESS_MODE_RM1;
      break;
    case TOGGLE_MODE:
      mode = (rmVersion == 2 && !forceRM1) ? TOGGLE_MODE_RM2 : TOGGLE_MODE_RM1;
      break;
  }

  if (!doubleClickAction) {
    printf("No double click action specified! Using default.\nDOUBLE CLICK ACTION: UNDO\n");
    doubleClickAction = UNDO;
  }
  int tripleClickAction = REDO;

  //main loop body
  for (;;) {
    read(fd_pen, &ev_pen, input_event_size); // note: read pauses until there is data
    trigger = getTrigger(&ev_pen);

    printTriggers(trigger, false);

    if (trigger == TRIPLE_CLICK) { //currently hardcoded to be REDO
        switch (tripleClickAction) {
        case WRITING:
          printf("writing write\n");
          actionWriting(fd_touch, rmVersion);
            break;
        case UNDO:
          printf("writing undo\n");
          actionUndo(fd_touch, rmVersion);
          break;
        case REDO:
          printf("writing redo\n");
          actionRedo(fd_touch, rmVersion);
          break;
        case SELECT:
          printf("writing select\n");
          toggleToolSelect(fd_touch, rmVersion);
          break;
        }
      }

    if (trigger == DOUBLE_CLICK) {
      switch (doubleClickAction) {
      case WRITING:
        printf("writing write\n");
        actionWriting(fd_touch, rmVersion);
          break;
      case UNDO:
        printf("writing undo\n");
        actionUndo(fd_touch, rmVersion);
        break;
      case REDO:
        printf("writing redo\n");
        actionRedo(fd_touch, rmVersion);
        break;
      case SELECT:
        printf("writing select\n");
        toggleToolSelect(fd_touch, rmVersion);
        break;
      }
    }

  switch(mode) {
    case PRESS_MODE_RM2:
      switch(trigger) {
        case PRESS_HOLD_ON :
          activateToolEraserRM2(fd_pen);
          break;
        case PRESS_HOLD_OFF :
          deactivateToolEraserRM2(fd_pen);
          break;
        }
      break;
    case PRESS_MODE_RM1:
      switch(trigger) {
        case PRESS_HOLD_ON :
          activateToolEraserRM1(fd_touch, rmVersion);
          break;
        case PRESS_HOLD_OFF :
          deactivateToolEraserRM1(fd_touch, rmVersion);
          break;
        }
      break;
    case TOGGLE_MODE_RM2:
      if (trigger == CLICK)
          toggleToolEraserRM2(fd_pen);
      break;
    case TOGGLE_MODE_RM1:
      if (trigger == CLICK)
          toggleToolEraserRM1(fd_touch, rmVersion);
      break;
    }



    actionToolEraserRM2(&ev_pen, fd_pen);

    // (these groupings for future use)
    //RUN ONCE TYPE ACTIONS: code can run exactly once to perform action.
    //MARKER, UNDO, REDO

    //LOOP TYPE ACTIONS: code must loop to perform action
    //ERASE, ERASE-SELECT




  }
  return EXIT_SUCCESS;
}
