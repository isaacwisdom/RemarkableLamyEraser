#include <linux/input.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>


#include "screenlocations.h"
#include "orientation.h"
#include "actions.h"


void writeEvent(int fd, struct input_event event) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  event.time = tv;
  // debug: printf("writing: seconds = %ld, usec= %ld, type = %d, code = %d, value = %d\n", event.time.tv_sec, event.time.tv_usec, event.type, event.code, event.value);
  write(fd, &event, sizeof(struct input_event));
}

void writeTapWithTouch(int fd, const int location[]) {
  struct input_event event;

  // this is the minimum (probably) seqeunce of events that must be sent to tap the screen in a location.
  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_SLOT, .value = 0x7FFFFFFF}; // Use max signed int slot
  // printf("Writing ABS_MT_SLOT: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event){
      .type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = time(NULL)};
  // printf("Writing Tracking ID: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event){
      .type = EV_ABS, .code = ABS_MT_POSITION_X, .value = location[0]};
  // printf("Writing Touch X: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event){
      .type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = location[1]};
  // printf("Writing Touch Y: %d\n", event.value);
  writeEvent(fd, event);

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  // printf("Writing SYN Report\n");
  writeEvent(fd, event);

  event = (struct input_event){
      .type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = -1};
  // printf("Writing Tracking ID: -1\n");
  writeEvent(fd, event);

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  // printf("Writing SYN Report\n");
  writeEvent(fd, event);
}

void toggleMode(struct input_event ev, int fd) {
  static int toggle = 0;
  if (ev.code == BTN_STYLUS &&
      ev.value == 1) { // change state of toggle on button press
    toggle = !toggle;
    printf("toggle: %d \n", toggle);
  }
  if (toggle)
    if (ev.code == BTN_TOOL_PEN) { // when toggle is on, we write these events
                                   // following the pen state
      if (ev.value == 1) {
        printf("writing eraser on\n");
        writeEvent(fd, tool_rubber_on);
      } else {
        printf("writing eraser off\n");
        writeEvent(fd, tool_rubber_off);
      }
    }
}

void pressMode(struct input_event ev, int fd) {
  if (ev.code == BTN_STYLUS) {
    ev.code = BTN_TOOL_RUBBER; // value will follow the button, so we can reuse
                               // the message
    writeEvent(fd, ev);
  }
}

bool doublePressHandler(struct input_event ev) { // returns true if a double press has happened
  static struct timeval prevTime;
  const double maxDoublePressTime = .5; // 500ms seems to be the standard double press time
  double elapsedTime = (ev.time.tv_sec + ev.time.tv_usec / 1000000.0) - (prevTime.tv_sec + prevTime.tv_usec / 1000000.0);
  if (ev.code == BTN_STYLUS && ev.value == 1) {
    // printf("Current Time: %f | ", ev.time.tv_sec + ev.time.tv_usec/1000000.0); printf("Prev Time: %f |", prevTime.tv_sec + prevTime.tv_usec/1000000.0);
    // printf("Elapsed Time: %f\n", elapsedTime);
    if (elapsedTime <= maxDoublePressTime) {
      return true;
    }
    prevTime = ev.time;
  }
  return false;
}

void actionUndo(int fd) {
  toolbarOrientation orientation = getToolbarOrientation();
  if(orientation.openNotebook) {
      if (orientation.rightHanded) { // RH
        if (orientation.portrait) {  // RH, Portrait
          writeTapWithTouch(fd, RHPundo);
          writeTapWithTouch(fd, RHPpanel);
          writeTapWithTouch(fd, RHPundo);
          writeTapWithTouch(fd, RHPpanel);
        } else { // RH, Landscape
          writeTapWithTouch(fd, RHLundo);
          writeTapWithTouch(fd, RHLpanel);
          writeTapWithTouch(fd, RHLundo);
          writeTapWithTouch(fd, RHLpanel);
        }
      } else {          // LH
        if (orientation.portrait) { // LH, Portrait
          writeTapWithTouch(fd, LHPundo);
          writeTapWithTouch(fd, LHPpanel);
          writeTapWithTouch(fd, LHPundo);
          writeTapWithTouch(fd, LHPpanel);
        } else { // LH, Landscape
          writeTapWithTouch(fd, LHLundo);
          writeTapWithTouch(fd, LHLpanel);
          writeTapWithTouch(fd, LHLundo);
          writeTapWithTouch(fd, LHLpanel);
        }
      }
  }
}

void actionRedo(int fd) {
  toolbarOrientation orientation = getToolbarOrientation();
  if(orientation.openNotebook) {
      if (orientation.rightHanded) { // RH
        if (orientation.portrait) {  // RH, Portrait
          writeTapWithTouch(fd, RHPredo);
          writeTapWithTouch(fd, RHPpanel);
          writeTapWithTouch(fd, RHPredo);
          writeTapWithTouch(fd, RHPpanel);
        } else { // RH, Landscape
          writeTapWithTouch(fd, RHLredo);
          writeTapWithTouch(fd, RHLpanel);
          writeTapWithTouch(fd, RHLredo);
          writeTapWithTouch(fd, RHLpanel);
        }
      } else {          // LH
        if (orientation.portrait) { // LH, Portrait
          writeTapWithTouch(fd, LHPredo);
          writeTapWithTouch(fd, LHPpanel);
          writeTapWithTouch(fd, LHPredo);
          writeTapWithTouch(fd, LHPpanel);
        } else { // LH, Landscape
          writeTapWithTouch(fd, LHLredo);
          writeTapWithTouch(fd, LHLpanel);
          writeTapWithTouch(fd, LHLredo);
          writeTapWithTouch(fd, LHLpanel);
        }
      }
  }
}
