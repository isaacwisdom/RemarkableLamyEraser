#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

#include <screenlocations.h>

#define PEN_DEVICE      "/dev/input/event1"
#define TOUCH_DEVICE    "/dev/input/event2"


//const struct input_event tool_touch_off = { .type = EV_KEY, .code = BTN_TOUCH, .value = 0}; //these might be used in the future to improve press and hold mode
//const struct input_event tool_pen_on = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 1}; //used when pen approaches the screen
//const struct input_event tool_pen_off = { .type = EV_KEY, .code = BTN_TOOL_PEN, .value = 0};
const struct input_event tool_rubber_on = { .type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 1}; //used when rubber approaches the screen
const struct input_event tool_rubber_off = { .type = EV_KEY, .code = BTN_TOOL_RUBBER, .value = 0};


void writeEvent(int fd, struct input_event event)
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  event.time = tv;
  //debug: printf("writing: seconds = %ld, usec= %ld, type = %d, code = %d, value = %d\n", event.time.tv_sec, event.time.tv_usec, event.type, event.code, event.value);
  write(fd, &event, sizeof(struct input_event));
}

void writeTapWithTouch(int fd, int location[2]) {
  struct input_event event;

  //this is the minimum (probably) seqeunce of events that must be sent to tap the screen in a location.
  event = (struct input_event) {.type = EV_ABS, .code = ABS_MT_SLOT, .value = 0x7FFFFFFF}; //Use max signed int slot
  //printf("Writing ABS_MT_SLOT: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event) {.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = time(NULL)};
  //printf("Writing Tracking ID: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event) {.type = EV_ABS, .code = ABS_MT_POSITION_X, .value = location[0]};
  //printf("Writing Touch X: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event) {.type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = location[1]};
  //printf("Writing Touch Y: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event) {.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  //printf("Writing SYN Report\n");
  writeEvent(fd, event);

  event = (struct input_event) {.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = -1};
  //printf("Writing Tracking ID: -1\n");
  writeEvent(fd, event);

  event = (struct input_event) {.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  //printf("Writing SYN Report\n");
  writeEvent(fd, event);
}

void toggleMode(struct input_event ev, int fd) {
  static int toggle = 0;
  if (ev.code == BTN_STYLUS && ev.value == 1) { // change state of toggle on button press
      toggle = !toggle;
      printf("toggle: %d \n", toggle);
    }
  if (toggle)
    if (ev.code == BTN_TOOL_PEN) { //when toggle is on, we write these events following the pen state
        if (ev.value == 1) {
          printf("writing eraser on\n");
          writeEvent(fd, tool_rubber_on);
          }
        else {
            printf("writing eraser off\n");
            writeEvent(fd, tool_rubber_off);
        }
      }
}


void pressMode(struct input_event ev, int fd) {
  if (ev.code == BTN_STYLUS) {
      ev.code = BTN_TOOL_RUBBER; //value will follow the button, so we can reuse the message
      writeEvent(fd, ev);
    }
}

void toggleModeRM1 (struct input_event ev, int fd) { //TODO: FIX THIS!!!
  static int toggle = 0;
  static int actionComplete = 0;
  if (ev.code == BTN_STYLUS && ev.value == 1) { // change state of toggle on button press
      toggle = !toggle;
      actionComplete = 0;
      printf("toggle: %d \n", toggle);
    }
  if (toggle && !actionComplete) {
        printf("writing eraser on\n");
        writeTapWithTouch(fd, eraserTouch);
        writeTapWithTouch(fd, panelTouch);
        writeTapWithTouch(fd, eraserTouch);
        writeTapWithTouch(fd, panelTouch);
        actionComplete = 1;
          }
  else if (!actionComplete){
        printf("writing eraser off\n");
        writeTapWithTouch(fd, markerTouch);
        writeTapWithTouch(fd, panelTouch);
        writeTapWithTouch(fd, markerTouch);
        writeTapWithTouch(fd, panelTouch);
        actionComplete = 1;
    }
}

void pressModeRM1(struct input_event ev, int fd) {
  if (ev.code == BTN_STYLUS && ev.value == 1) {
      printf("writing eraser_tool on...\n");
      writeTapWithTouch(fd, eraserTouch);
      writeTapWithTouch(fd, panelTouch);
      writeTapWithTouch(fd, eraserTouch);
      writeTapWithTouch(fd, panelTouch);
    }
  else if (ev.code == BTN_STYLUS && ev.value == 0) {
      printf("writing marker tool on...\n");
      writeTapWithTouch(fd, markerTouch);
      writeTapWithTouch(fd, panelTouch);
      writeTapWithTouch(fd, markerTouch);
      writeTapWithTouch(fd, panelTouch);
    }
}
bool doublePressHandler(struct input_event ev) { //returns true if a double press has happened
  static struct timeval prevTime;
  const double maxDoublePressTime = .5;
  double elapsedTime = (ev.time.tv_sec + ev.time.tv_usec/1000000.0) - (prevTime.tv_sec + prevTime.tv_usec/1000000.0);
  if (ev.code == BTN_STYLUS && ev.value == 1) {
      //printf("Current Time: %f | ", ev.time.tv_sec + ev.time.tv_usec/1000000.0);
      //printf("Prev Time: %f |", prevTime.tv_sec + prevTime.tv_usec/1000000.0);
      //printf("Elapsed Time: %f\n", elapsedTime);
      if (elapsedTime <= maxDoublePressTime) {
        return true;
      }
      prevTime = ev.time;
    }
  return false;
}

bool doublePressToggler(int doublePressHandler) { //returns a toggle based on input. Should be fed doublePressHandler as input
  static int toggle = 0;
  if (doublePressHandler) {
    toggle = !toggle;
    }
  return toggle;
}

int main(int argc, char *argv[]) {
    int mode = 0, doublePressAction = 0;
    struct input_event ev_pen;
    int fd_pen, fd_touch;

    char name[256] = "Unknown";

    //check our input args
    for(int i = 1; i < argc; i++) {
        if (!strncmp(argv[i], "--toggle", 8)) {
            printf("MODE: TOGGLE\n");
            mode = TOGGLE_MODE;
         }
        else if (!strncmp(argv[i], "--press", 7)) {
            printf("MODE: PRESS AND HOLD\n");
            mode = PRESS_MODE;
          }
        else if (!strncmp(argv[i], "--double-press", 14)) {
            if (!strncmp(argv[i+1], "undo", 4)) {
                printf("DOUBLE CLICK ACTION: UNDO\n");
                doublePressAction = UNDO;
                i++; //manually increment i so we skip interpretting the double press action arg
              }
            else if (!strncmp(argv[i+1], "redo", 4)) {
                printf("DOUBLE CLICK ACTION: REDO\n");
                doublePressAction = REDO;
                i++; //manually increment i so we skip interpretting the double press action arg
              }
            else if (!strncmp(argv[i+1], "eraser-tool", 11)) {
                printf("DOUBLE CLICK ACTION: ERASER TOOL\n");
                doublePressAction = ERASER_TOOL;
                i++; //manually increment i so we skip interpretting the double press action arg
              }
            else if (!strncmp(argv[i+1], "erase-selection", 15)) {
                printf("DOUBLE CLICK ACTION: ERASE SELECTION\n");
                doublePressAction = ERASE_SELECTION_TOOL;
                i++; //manually increment i so we skip interpretting the double press action arg
              }
            else if (!strncmp(argv[i+1], "select-tool", 11)) {
                printf("DOUBLE CLICK ACTION: SELECT TOOL\n");
                doublePressAction = SELECT_TOOL;
                i++; //manually increment i so we skip interpretting the double press action arg
              }
            else {
                printf("Unknown double press action <%s>. Using default.\n", argv[i+1]);
                printf("DOUBLE CLICK ACTION: UNDO\n");
                doublePressAction = UNDO;
              }
          }
        else {
            printf("Unknown argument %s\nExiting...\n", argv[i]);
            exit(EXIT_FAILURE);
          }
      }
    if (!mode) {
        printf("No mode specified! Using default.\nMODE: PRESS AND HOLD\n");
        mode = PRESS_MODE;
       }
    if (!doublePressAction) {
        printf("No double click action specified! Using default.\nDOUBLE CLICK ACTION: UNDO\n");
        doublePressAction = UNDO;
      }

    /* Open Device: Pen */
    fd_pen = open(PEN_DEVICE, O_RDWR);
    if (fd_pen == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE);
        exit(EXIT_FAILURE);
      }
    /* Open Device: Touch */
    fd_touch = open(TOUCH_DEVICE, O_WRONLY);
    if (fd_touch == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE);
        exit(EXIT_FAILURE);
      }


    /* Print Device Name */
    ioctl(fd_pen, EVIOCGNAME(sizeof(name)), name);
    printf("Using Devices:\n");
    printf("1. device file = %s\n", PEN_DEVICE);
    printf("   device name = %s\n", name);
    ioctl(fd_touch, EVIOCGNAME(sizeof(name)), name);
    printf("2. device file = %s\n", TOUCH_DEVICE);
    printf("   device name = %s\n", name);

    for (;;) {
        const size_t ev_pen_size = sizeof(struct input_event);
        read(fd_pen, &ev_pen, ev_pen_size); //note: read pauses until there is data

        if(doublePressHandler(ev_pen)) {
          printf("double click detected\n");
          switch(doublePressAction) {
            case UNDO :
              printf("writing undo\n");
              writeTapWithTouch(fd_touch, undoTouch);
              writeTapWithTouch(fd_touch, panelTouch);
              writeTapWithTouch(fd_touch, undoTouch);
              writeTapWithTouch(fd_touch, panelTouch);  //doing it like this ensures that the panel returns to it's starting state
              break;
            case REDO :
              printf("writing redo\n");
              writeTapWithTouch(fd_touch, redoTouch);
              writeTapWithTouch(fd_touch, panelTouch);
              writeTapWithTouch(fd_touch, redoTouch);
              writeTapWithTouch(fd_touch, panelTouch);
              break;
            case ERASER_TOOL :
              printf("writing redo\n");
              writeTapWithTouch(fd_touch, eraserTouch);
              writeTapWithTouch(fd_touch, panelTouch);
              writeTapWithTouch(fd_touch, eraserTouch);
              writeTapWithTouch(fd_touch, panelTouch);
              break;
            case ERASE_SELECTION_TOOL :
              printf("writing erase selection\n");
              printf("TODO: add in ERASE_SELECTION handling.");
              break;
            case SELECT_TOOL :
              printf("writing select tool\n");
              printf("TODO: add in SELECT_TOOL handling.");
              break;
            }
          }

        switch(mode) {
          case TOGGLE_MODE :
            toggleMode(ev_pen, fd_pen);
            break;
          case PRESS_MODE :
            pressMode(ev_pen, fd_pen);
            break;
          default :
            printf("Somehow a mode wasn't set? Exiting...\n");
            exit(EXIT_FAILURE);
          }
      }
    return EXIT_SUCCESS;
}
