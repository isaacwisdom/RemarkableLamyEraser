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
  bool trigger_debug = false;
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
    } else if (!strcmp(argv[i], "--trigger-debug")) {
      trigger_debug = true;
    } else {
      printf("Unknown argument %s. Valid options are:\n"
             "--config-file </path/to/config>\n"
             "--test-locations DEVICE (WACOM or TOUCH).\n"
             "Exiting...\n",
             argv[i]);
      exit(EXIT_FAILURE);
    }
  }
  if (!trigger_debug) {
    printf("Using configuration file at %s\n", confPath);
    if (get_trigger_config(confPath, &config) != 0) {
      printf("Reading configuration file failed. Exiting...\n");
      exit(EXIT_FAILURE);
    }
    printf("----------------------------------\n");
  }

  int flags = fcntl(fd_touch, F_GETFL, 0);
  fcntl(fd_touch, F_SETFL, flags | O_NONBLOCK);

  setAssumeTBOpen(config.assumeTBOpen);

  bool contact = false; // is the pen currently touching the screen?
  bool held = false; // is the pen button currently registered as held?

  // Main loop body
  for (;;) {
    if (read(fd_touch, &ev_touch, input_event_size)) { // non-blocking
      handle_current_tracking_ID(&ev_touch);
      // you can get a continous stream of touch events by holding the pen close to the screen while tapping with your finger
      if (ev_touch.code == ABS_MT_POSITION_X || ev_touch.code == ABS_MT_POSITION_Y) {
        // printf("event: %x, %d\n", ev_touch.code, ev_touch.value);
      }
    }
    // Note: read pauses until there is data
    read(fd_wacom, &ev_wacom, input_event_size);
    // printf("event: %x, %d\n", ev_wacom.code, ev_wacom.value);
    trigger = get_triggger(&ev_wacom);
    
    if (trigger_debug) continue;
    // printTriggers(trigger, false);

    int trigger_type = trigger & 0xf0;
    int trigger_count = trigger & 0x0f;

    enum effect_type etype = temp_on;

    if (trigger_type == CLICK || trigger_type == LCLICK) etype = toggle;
    else if (trigger_type == HOLD_OFF || trigger_type == PEN_UP) etype = temp_off;

    bool ingoreTrigger = (trigger_type == HOLD_ON && contact) || (trigger_type == HOLD_OFF && !held);

    effect = NULL_EFFECT;

    if (!ingoreTrigger) {
      switch (trigger_type) {
        case CLICK: effect = config.clickEffect[trigger_count - 1]; break;

        case LCLICK: effect = config.longClickEffect[trigger_count - 1]; break;

        case HOLD_ON: effect = config.holdEffect[trigger_count - 1]; held = true; break;

        case HOLD_OFF:
          if (!contact) effect = temp_effect_end(); // defer disabling temporary effect to PEN_UP if pen is still on screen
          held = false;
          break;

        case PEN_DOWN: contact = true; break;

        case PEN_UP:
          if (!held) effect = temp_effect_end(); // defer disabling temporary effect to HOLD_OFF if pen button is still held
          contact = false;
          break;
      }
    }

    switch (effect) {
      case ONE_OFF_HL:
        printf("one-off highlighter\n");
        effect = WRITING_HL;
        etype = one_off();
        break;
      case ONE_OFF_ERASER:
        printf("one-off eraser\n");
        effect = ERASER_ERASE;
        etype = one_off();
        break;
      case ONE_OFF_ERASER_SELECTION:
        printf("one-off eraser select\n");
        effect = ERASER_SELECTION;
        etype = one_off();
        break;
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

      // toggle tools
      case WRITING_HL:
        hl(etype, fd_touch);
        break;

      case ERASER_ERASE:
        tool_eraser(etype, fd_wacom);
        break;

      case ERASER_SELECTION:
        tool_eraser_select(etype, fd_touch);
        break;

      case SELECT:
        tool_select(etype, fd_touch);
        break;
    }
    action_tool_eraser(&ev_wacom, fd_wacom);
  }
  return EXIT_SUCCESS;
}
