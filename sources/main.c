#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "configuration.h"
#include "effects.h"
#include "effects_data.h"
#include "orientation.h"
#include "triggers.h"

#define WACOM_DEVICE "/dev/input/event1"
#define TOUCH_DEVICE "/dev/input/event2"

int main(int argc, char *argv[]) {
  struct configuration config;
  char *confPath = "/home/root/.config/LamyEraser/LamyEraser.conf"; // default conf path
  int   trigger = NULL_TRIGGER, effect = NULL_EFFECT;
  struct input_event ev_wacom, ev_touch;
  const size_t       input_event_size = sizeof(struct input_event);
  int                fd_wacom, fd_touch;
  char               name[256] = "Unknown";
  char               wacom_device_path[18], touch_device_path[18];
  int                rm_version    = get_rm_version();
  int                sw_version[4] = {0, 0, 0, 0};
  get_software_version(sw_version);

  printf("RemarkableLamyEraser 2.2.0\n");
  printf("----------------------------------\n");

  /* Quit if software version is less than 3.0 */
  if (sw_version[0] < 3) {
    printf("Invalid software version: %d.%d.%d.%d/n"
           "This version of RemarkableLamyEraser must be run on devices "
           "running software version 3.0.0.0 or newer",
           sw_version[0], sw_version[1], sw_version[2], sw_version[3]);
    exit(EXIT_FAILURE);
  }

  /* Open Input Devices */
  if (rm_version == 2) {
    strcpy(wacom_device_path, WACOM_DEVICE);
    strcpy(touch_device_path, TOUCH_DEVICE);
  } else {
    printf("Invalid reMarkable Version: %d. Exiting...\n", rm_version);
    exit(EXIT_FAILURE);
  }
  fd_wacom = open(wacom_device_path, O_RDWR);
  fd_touch = open(touch_device_path, O_RDWR);
  if (fd_wacom == -1) {
    fprintf(stderr, "%s is not a vaild device. Exiting...\n", wacom_device_path);
    exit(EXIT_FAILURE);
  }
  if (fd_touch == -1) {
    fprintf(stderr, "%s is not a vaild device. Exiting...\n", touch_device_path);
    exit(EXIT_FAILURE);
  }

  /* Print Device Names */
  printf("Detected ReMarkable %d, Software Version: %d.x.x.x\n", rm_version,
         sw_version[0]);
  ioctl(fd_wacom, EVIOCGNAME(sizeof(name)), name);
  printf("Using Devices:\n");
  printf("1. device file = %s\n", wacom_device_path);
  printf("   device name = %s\n", name);
  ioctl(fd_touch, EVIOCGNAME(sizeof(name)), name);
  printf("2. device file = %s\n", touch_device_path);
  printf("   device name = %s\n", name);
  printf("----------------------------------\n");

  // check our input args
  for (int i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "--config-file")) {
      confPath = argv[++i];
    } else if (!strcmp(argv[i], "--test-locations")) {
      test_locations(fd_touch); // program will exit
    } else {
      printf("Unknown argument %s. Valid options are:\n"
             "--config-file </path/to/config>\n"
             "--test-locations DEVICE (WACOM or TOUCH).\n"
             "Exiting...\n",
             argv[i]);
      exit(EXIT_FAILURE);
    }
  }

  printf("Using configuration file at %s\n", confPath);
  if (get_trigger_config(confPath, &config) != 0) {
    printf("Reading configuration file failed. Exiting...\n");
    exit(EXIT_FAILURE);
  }
  printf("----------------------------------\n");

  int temp;

#define TOGGLE temp == ERASER_ERASE || temp == ERASER_SELECTION || temp == SELECT || temp == WRITING_HL

  temp = config.click1Effect;
  if (TOGGLE)
    config.click1Effect += TOGGLE_OFFSET;

  temp = config.click2Effect;
  if (TOGGLE)
    config.click2Effect += TOGGLE_OFFSET;

  temp = config.click3Effect;
  if (TOGGLE)
    config.click3Effect += TOGGLE_OFFSET;

  temp = config.click4Effect;
  if (TOGGLE)
    config.click4Effect += TOGGLE_OFFSET;

  temp = config.click5Effect;
  if (TOGGLE)
    config.click5Effect += TOGGLE_OFFSET;

  int flags = fcntl(fd_touch, F_GETFL, 0);
  fcntl(fd_touch, F_SETFL, flags | O_NONBLOCK);

  // Main loop body
  for (;;) {
    if (read(fd_touch, &ev_touch, input_event_size)) { // non-blocking
      handle_current_tracking_ID(&ev_touch);
    }
    // Note: read pauses until there is data
    read(fd_wacom, &ev_wacom, input_event_size);
    trigger = get_triggger(&ev_wacom);
    // printTriggers(trigger, false);

    switch (trigger) {
      case CLICK_1: effect = config.click1Effect; break;
      case CLICK_2: effect = config.click2Effect; break;
      case CLICK_3: effect = config.click3Effect; break;
      case CLICK_4: effect = config.click4Effect; break;
      case CLICK_5: effect = config.click5Effect; break;

      case HOLD_1_ON: effect = config.hold1Effect; break;
      case HOLD_2_ON: effect = config.hold2Effect; break;
      case HOLD_3_ON: effect = config.hold3Effect; break;
      case HOLD_4_ON: effect = config.hold4Effect; break;
      case HOLD_5_ON: effect = config.hold5Effect; break;

      case HOLD_1_OFF: effect = config.hold1Effect + HOLD_OFF_OFFSET; break;
      case HOLD_2_OFF: effect = config.hold2Effect + HOLD_OFF_OFFSET; break;
      case HOLD_3_OFF: effect = config.hold3Effect + HOLD_OFF_OFFSET; break;
      case HOLD_4_OFF: effect = config.hold4Effect + HOLD_OFF_OFFSET; break;
      case HOLD_5_OFF: effect = config.hold5Effect + HOLD_OFF_OFFSET; break;
      default:
        effect = NULL_EFFECT;
    }

    /*if(effect != 0)
      printf("effect: %x\n", effect);
    */

    switch (effect) {
      // actions here
      case TOOLBAR:
        printf("writing toolbar\n");
        action_toolbar(fd_touch);
        break;
      case WRITING:
        printf("writing write\n");
        action_writing(fd_touch);
        break;
      case TEXT:
        printf("writing text\n");
        action_text(fd_touch);
        break;
      case ERASER_PANEL:
        printf("writing eraser panel\n");
        action_eraser_panel(fd_touch);
        break;
      case UNDO:
        printf("writing undo\n");
        action_undo(fd_touch);
        break;
      case REDO:
        printf("writing redo\n");
        action_redo(fd_touch);
        break;
      case WRITING_FINELINER:
        printf("writing fineliner pen\n");
        action_fineliner(fd_touch);
        break;
      case WRITING_CALLIGRAPHY:
        printf("writing calligraphy pen\n");
        action_calligraphy(fd_touch);
        break;
      case WRITING_BLACK:
        printf("writing black colour\n");
        action_black(fd_touch);
        break;
      case WRITING_GREY:
        printf("writing grey colour\n");
        action_grey(fd_touch);
        break;
      case WRITING_HL:
        printf("writing highlighter\n");
        action_hl(fd_touch);
        break;
      case HL_TOGGLE:
        printf("toggle highlighter\n");
        toggle_hl(fd_touch);
        break;

      // tools here
      case ERASER_ERASE:
        printf("writing eraser [on]\n");
        activate_tool_eraser(fd_wacom);
        break;
      case ERASER_ERASE_OFF:
        printf("writing eraser [off]\n");
        deactivate_tool_eraser(fd_wacom);
        break;
      case ERASER_ERASE_TOGGLE:
        printf("writing eraser [toggle]\n");
        toggle_tool_eraser(fd_wacom);
        break;

      case ERASER_SELECTION:
        printf("writing erase selection [on]\n");
        activate_tool_eraser_select(fd_touch);
        break;
      case ERASER_SELECTION_OFF:
        printf("writing erase selection [off]\n");
        deactivate_tool_eraser_select(fd_touch);
        break;
      case ERASER_SELECT_TOGGLE:
        printf("writing erase selection [toggle]\n");
        toggle_tool_eraser_select(fd_touch);
        break;

      case SELECT:
        printf("writing select [on]\n");
        activate_tool_select(fd_touch);
        break;
      case SELECT_OFF:
        printf("writing select [off]\n");
        deactivate_tool_select(fd_touch);
        break;
      case SELECT_TOGGLE:
        printf("writing select [toggle]\n");
        toggle_tool_select(fd_touch);
        break;
    }
    action_tool_eraser(&ev_wacom, fd_wacom);
  }
  return EXIT_SUCCESS;
}
