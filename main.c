#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "triggers.h"
#include "effects.h"
#include "orientation.h"
#include "configuration.h"

#define PEN_DEVICE_RM2   "/dev/input/event1"
#define TOUCH_DEVICE_RM2 "/dev/input/event2"
#define PEN_DEVICE_RM1   "/dev/input/event0"
#define TOUCH_DEVICE_RM1 "/dev/input/event1"

int main(int argc, char *argv[]) {
  printf("RemarkableLamyEraser 2.0\n");
  printf("----------------------------------\n");
  struct configuration config;
  char* confPath = "/home/root/.config/LamyEraser/LamyEraser.conf"; //default conf path
  int trigger = NULL_TRIGGER, effect = NULL_EFFECT;
  struct input_event ev_pen;
  const size_t input_event_size = sizeof(struct input_event);
  int fd_pen, fd_touch;
  char name[256] = "Unknown";
  int forceRM1Style = 0;
  int rmVersion = getRmVersion();

  // check our input args
  if (argc > 2) {
      printf("Unknown arguments supplied. Exiting...\n");
    }
  else if (argc == 2)
    if ( !strcmp(argv[1], "--config-file") ) {
      confPath = argv[2];
    }

  printf("Using configuration file at %s\n", confPath);
  if ( getTriggerConfig(confPath, &config)  == -1)
      printf("Reading configuration file failed. Using default settings...\n");
  printf("----------------------------------\n");



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

  int temp;
  temp = config.click1Effect;
  if (temp == ERASER_ERASE || temp == ERASER_SELECT || temp == SELECT)
    config.click1Effect += TOGGLE_OFFSET;
  temp = config.click2Effect;
  if (temp == ERASER_ERASE || temp == ERASER_SELECT || temp == SELECT)
    config.click2Effect += TOGGLE_OFFSET;
  temp = config.click3Effect;
  if (temp == ERASER_ERASE || temp == ERASER_SELECT || temp == SELECT)
    config.click3Effect += TOGGLE_OFFSET;
  temp = config.click4Effect;
  if (temp == ERASER_ERASE || temp == ERASER_SELECT || temp == SELECT)
    config.click4Effect += TOGGLE_OFFSET;
  temp = config.click5Effect;
  if (temp == ERASER_ERASE || temp == ERASER_SELECT || temp == SELECT)
    config.click5Effect += TOGGLE_OFFSET;

  printConfig(&config);


  //main loop body
  for (;;) {
    read(fd_pen, &ev_pen, input_event_size); // note: read pauses until there is data
    trigger = getTrigger(&ev_pen);

    printTriggers(trigger, false);

    switch(trigger) {
      case CLICK_1 :
      effect = config.click1Effect;
      break;
      case CLICK_2 :
      effect = config.click2Effect;
      break;
      case CLICK_3 :
      effect = config.click3Effect;
      break;
      case CLICK_4 :
      effect = config.click4Effect;
      break;
      case CLICK_5 :
      effect = config.click5Effect;
      break;

      case HOLD_1_ON :
      effect = config.hold1Effect;
      break;
      case HOLD_2_ON :
      effect = config.hold2Effect;
      break;
      case HOLD_3_ON :
      effect = config.hold3Effect;
      break;
      case HOLD_4_ON :
      effect = config.hold4Effect;
      break;
      case HOLD_5_ON :
      effect = config.hold5Effect;
      break;

      case HOLD_1_OFF :
      effect = config.hold1Effect + HOLD_OFF_OFFSET;
      break;
      case HOLD_2_OFF :
      effect = config.hold2Effect + HOLD_OFF_OFFSET;
      break;
      case HOLD_3_OFF :
      effect = config.hold3Effect + HOLD_OFF_OFFSET;
      break;
      case HOLD_4_OFF :
      effect = config.hold4Effect + HOLD_OFF_OFFSET;
      break;
      case HOLD_5_OFF :
      effect = config.hold5Effect + HOLD_OFF_OFFSET;
      break;
      default :
        effect = NULL_EFFECT;
      }




    switch (effect) {
      //actions here
      case TOOLBAR:
        printf("writing toolbar\n");
        actionToolbar(fd_touch, rmVersion);
          break;
      case WRITING:
        printf("writing write\n");
        actionWriting(fd_touch, rmVersion);
          break;
      case COLOR_BLACK:
        printf("writing color black\n");
        actionColorBlack(fd_pen, rmVersion);
        break;
      case COLOR_GREY:
        printf("writing color grey\n");
        actionColorGrey(fd_pen, rmVersion);
        break;
      case COLOR_WHITE:
        printf("writing color white\n");
        actionColorWhite(fd_pen, rmVersion);
        break;
      case UNDO:
        printf("writing undo\n");
        actionUndo(fd_touch, rmVersion);
        break;
      case REDO:
        printf("writing redo\n");
        actionRedo(fd_touch, rmVersion);
        break;

      //tools here
      case ERASER_ERASE:
        printf("writing eraser\n");
        if (rmVersion == 1 || forceRM1Style) {
            activateToolEraserRM1(fd_touch, rmVersion);
          }
        else
            activateToolEraserRM2(fd_pen);
        break;
      case ERASER_ERASE_OFF:
        printf("writing eraser\n");
        if (rmVersion == 1 || forceRM1Style) {
            activateToolEraserRM1(fd_touch, rmVersion);
          }
        else
            deactivateToolEraserRM2(fd_pen);
        break;
      case ERASER_ERASE_TOGGLE:
        printf("writing eraser\n");
        if (rmVersion == 1 || forceRM1Style) {
            toggleToolEraserRM1(fd_touch, rmVersion);
          }
        else
            toggleToolEraserRM2(fd_pen);
        break;

      case ERASER_SELECT:
        printf("writing erase selection\n");
        activateToolEraseSelect(fd_touch, rmVersion);
        break;
      case ERASER_SELECT_OFF:
        printf("writing erase selection off\n");
        deactivateToolEraseSelect(fd_touch, rmVersion);
        break;
      case ERASER_SELECT_TOGGLE:
        printf("writing erase selection off\n");
        toggleToolEraseSelect(fd_touch, rmVersion);
        break;

      case SELECT:
        printf("writing select\n");
        activateToolSelect(fd_touch, rmVersion);
        break;
      case SELECT_OFF:
        printf("writing select\n");
        deactivateToolSelect(fd_touch, rmVersion);
        break;
      case SELECT_TOGGLE:
        printf("writing select\n");
        toggleToolSelect(fd_touch, rmVersion);
        break;
      }

  if (rmVersion == 2)
    actionToolEraserRM2(&ev_pen, fd_pen);

  }
  return EXIT_SUCCESS;
}
