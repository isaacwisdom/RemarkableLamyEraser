#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "actions.h"
#include "orientation.h"
#include "screenlocations.h"

void writeEvent(int fd, struct input_event event) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  event.time = tv;
  // debug: printf("writing: seconds = %ld, usec= %ld, type = %d, code = %d,
  // value = %d\n", event.time.tv_sec, event.time.tv_usec, event.type,
  // event.code, event.value);
  write(fd, &event, sizeof(struct input_event));
}

void writeTapWithTouch(int fd_touch, const int location[]) {
  struct input_event event;

  // this is the minimum (probably) seqeunce of events that must be sent to tap
  // the screen in a location.
  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_SLOT, .value = 0x7FFFFFFF}; // Use max signed int slot
  // printf("Writing ABS_MT_SLOT: %d\n", event.value);
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = time(NULL)};
  // printf("Writing Tracking ID: %d\n", event.value);
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_POSITION_X, .value = location[0]};
  // printf("Writing Touch X: %d\n", event.value);
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_POSITION_Y, .value = location[1]};
  // printf("Writing Touch Y: %d\n", event.value);
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  // printf("Writing SYN Report\n");
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = -1};
  // printf("Writing Tracking ID: -1\n");
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  // printf("Writing SYN Report\n");
  writeEvent(fd_touch, event);
}



bool doublePressHandler(struct input_event ev_pen) {
  // returns true if a double press has happened
  static struct timeval prevTime;
  const double maxDoublePressTime =
      .5; // 500ms seems to be the standard double press time
  double elapsedTime = (ev_pen.time.tv_sec + ev_pen.time.tv_usec / 1000000.0) -
                       (prevTime.tv_sec + prevTime.tv_usec / 1000000.0);
  if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) {
    // printf("Current Time: %f | ", ev.time.tv_sec +
    // ev.time.tv_usec/1000000.0); printf("Prev Time: %f |", prevTime.tv_sec +
    // prevTime.tv_usec/1000000.0); printf("Elapsed Time: %f\n", elapsedTime);
    if (elapsedTime <= maxDoublePressTime) {
      return true;
    }
    prevTime = ev_pen.time;
  }
  return false;
}

void toggleMode(struct input_event ev_pen, int fd_pen) {
  static int toggle = 0;
  if (ev_pen.code == BTN_STYLUS &&
      ev_pen.value == 1) { // change state of toggle on button press
    toggle = !toggle;
    printf("toggle: %d \n", toggle);
  }
  if (toggle)
    if (ev_pen.code == BTN_TOOL_PEN) { // when toggle is on, we write these events
                                   // following the pen state
      if (ev_pen.value == 1) {
        printf("writing eraser on\n");
        writeEvent(fd_pen, tool_rubber_on);
      } else {
        printf("writing eraser off\n");
        writeEvent(fd_pen, tool_rubber_off);
      }
    }
}

void pressMode(struct input_event ev_pen, int fd_pen) {
  if (ev_pen.code == BTN_STYLUS) {
    ev_pen.code = BTN_TOOL_RUBBER; // value will follow the button, so we can reuse
                               // the message
    writeEvent(fd_pen, ev_pen);
  }
}

void toggleModeRM1(struct input_event ev_pen, int fd_touch) { // TODO: FIX THIS!!!
  static int toggle = 0;
  static int actionComplete = 0;
  if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) { // change state of toggle on button press
    toggle = !toggle;
    actionComplete = 0;
    printf("toggle: %d \n", toggle);
  }
  if (toggle && !actionComplete) {
    printf("writing eraser on\n");
    writeTapWithTouch(fd_touch, RHPeraser);
    writeTapWithTouch(fd_touch, RHPtoolbar);
    writeTapWithTouch(fd_touch, RHPeraser);
    writeTapWithTouch(fd_touch, RHPtoolbar);
    actionComplete = 1;
  } else if (!actionComplete) {
    printf("writing eraser off\n");
    writeTapWithTouch(fd_touch, RHPwriting);
    writeTapWithTouch(fd_touch, RHPtoolbar);
    writeTapWithTouch(fd_touch, RHPwriting);
    writeTapWithTouch(fd_touch, RHPtoolbar);
    actionComplete = 1;
  }
}

void pressModeRM1(struct input_event ev_pen, int fd_touch) {
  struct timeval tv;
  toolbarOrientation orientation;
  if (ev_pen.code == BTN_STYLUS) {
      orientation = getToolbarOrientation();
    }
  else
    return;

  if (orientation.openNotebook) {
    if (orientation.rightHanded) { // RH
      if (orientation.portrait) {  // RH, Portrait
        if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) {
            gettimeofday(&tv, NULL);
            printf("%ld.%ld: ", tv.tv_sec, tv.tv_usec);
            printf("writing RHPeraser...\n");
            writeTapWithTouch(fd_touch, RHPeraser);
            writeTapWithTouch(fd_touch, RHPtoolbar);
            writeTapWithTouch(fd_touch, RHPeraser);
            writeTapWithTouch(fd_touch, RHPtoolbar);
        } else if (ev_pen.code == BTN_STYLUS && ev_pen.value == 0) {
            gettimeofday(&tv, NULL);
            printf("%ld.%ld: ", tv.tv_sec, tv.tv_usec);
            printf("writing RHPwriting...\n");
            writeTapWithTouch(fd_touch, RHPwriting);
            writeTapWithTouch(fd_touch, RHPtoolbar);
            writeTapWithTouch(fd_touch, RHPwriting);
            writeTapWithTouch(fd_touch, RHPtoolbar);
        }
      } else { // RH, Landscape
          if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) {
            printf("writing RHLeraser on...\n");
            writeTapWithTouch(fd_touch, RHLeraser);
            writeTapWithTouch(fd_touch, RHLtoolbar);
            writeTapWithTouch(fd_touch, RHLeraser);
            writeTapWithTouch(fd_touch, RHLtoolbar);
        } else if (ev_pen.code == BTN_STYLUS && ev_pen.value == 0) {
            printf("writing RHLwriting on...\n");
            writeTapWithTouch(fd_touch, RHLwriting);
            writeTapWithTouch(fd_touch, RHLtoolbar);
            writeTapWithTouch(fd_touch, RHLwriting);
            writeTapWithTouch(fd_touch, RHLtoolbar);
        }
      }
    } else {                      // LH
      if (orientation.portrait) { // LH, Portrait
        if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) {
          printf("writing RHLeraser on...\n");
          writeTapWithTouch(fd_touch, LHLeraser);
          writeTapWithTouch(fd_touch, LHLtoolbar);
          writeTapWithTouch(fd_touch, LHLeraser);
          writeTapWithTouch(fd_touch, LHLtoolbar);
        } else if (ev_pen.code == BTN_STYLUS && ev_pen.value == 0) {
          printf("writing RHLwriting on...\n");
          writeTapWithTouch(fd_touch, LHLwriting);
          writeTapWithTouch(fd_touch, LHLtoolbar);
          writeTapWithTouch(fd_touch, LHLwriting);
          writeTapWithTouch(fd_touch, LHLtoolbar);
        } else { // LH, Landscape
          if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) {
            printf("writing LHLeraser on...\n");
            writeTapWithTouch(fd_touch, LHLeraser);
            writeTapWithTouch(fd_touch, LHLtoolbar);
            writeTapWithTouch(fd_touch, LHLeraser);
            writeTapWithTouch(fd_touch, LHLtoolbar);
          } else if (ev_pen.code == BTN_STYLUS && ev_pen.value == 0) {
            printf("writing LHLmarker tool on...\n");
            writeTapWithTouch(fd_touch, LHLwriting);
            writeTapWithTouch(fd_touch, LHLtoolbar);
            writeTapWithTouch(fd_touch, LHLwriting);
            writeTapWithTouch(fd_touch, LHLtoolbar);
          }
        }
      }
    }
  }
}

/*
void actionTemplate(int fd) {
  //Be aware that getting the orientation is slow, and not ideal for actions that have to
  //read ev_pen data
  toolbarOrientation orientation = getToolbarOrientation();
  if(orientation.openNotebook) {
      if (orientation.rightHanded) { // RH
        if (orientation.portrait) {  // RH, Portrait
          // do action here...
        } else { // RH, Landscape
          // do action here...
        }
      } else {          // LH
        if (orientation.portrait) { // LH, Portrait
          // do action here...
        } else { // LH, Landscape
          // do action here...
        }
      }
  }
}
*/

void actionUndo(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  if (orientation.openNotebook) {
    if (orientation.rightHanded) { // RH
      if (orientation.portrait) {  // RH, Portrait
        writeTapWithTouch(fd_touch, RHPundo);
        writeTapWithTouch(fd_touch, RHPtoolbar);
        writeTapWithTouch(fd_touch, RHPundo);
        writeTapWithTouch(fd_touch, RHPtoolbar);
      } else { // RH, Landscape
        writeTapWithTouch(fd_touch, RHLundo);
        writeTapWithTouch(fd_touch, RHLtoolbar);
        writeTapWithTouch(fd_touch, RHLundo);
        writeTapWithTouch(fd_touch, RHLtoolbar);
      }
    } else {                      // LH
      if (orientation.portrait) { // LH, Portrait
        writeTapWithTouch(fd_touch, LHPundo);
        writeTapWithTouch(fd_touch, LHPtoolbar);
        writeTapWithTouch(fd_touch, LHPundo);
        writeTapWithTouch(fd_touch, LHPtoolbar);
      } else { // LH, Landscape
        writeTapWithTouch(fd_touch, LHLundo);
        writeTapWithTouch(fd_touch, LHLtoolbar);
        writeTapWithTouch(fd_touch, LHLundo);
        writeTapWithTouch(fd_touch, LHLtoolbar);
      }
    }
  }
}

void actionRedo(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  if (orientation.openNotebook) {
    if (orientation.rightHanded) { // RH
      if (orientation.portrait) {  // RH, Portrait
        writeTapWithTouch(fd_touch, RHPredo);
        writeTapWithTouch(fd_touch, RHPtoolbar);
        writeTapWithTouch(fd_touch, RHPredo);
        writeTapWithTouch(fd_touch, RHPtoolbar);
      } else { // RH, Landscape
        writeTapWithTouch(fd_touch, RHLredo);
        writeTapWithTouch(fd_touch, RHLtoolbar);
        writeTapWithTouch(fd_touch, RHLredo);
        writeTapWithTouch(fd_touch, RHLtoolbar);
      }
    } else {                      // LH
      if (orientation.portrait) { // LH, Portrait
        writeTapWithTouch(fd_touch, LHPredo);
        writeTapWithTouch(fd_touch, LHPtoolbar);
        writeTapWithTouch(fd_touch, LHPredo);
        writeTapWithTouch(fd_touch, LHPtoolbar);
      } else { // LH, Landscape
        writeTapWithTouch(fd_touch, LHLredo);
        writeTapWithTouch(fd_touch, LHLtoolbar);
        writeTapWithTouch(fd_touch, LHLredo);
        writeTapWithTouch(fd_touch, LHLtoolbar);
      }
    }
  }
}
