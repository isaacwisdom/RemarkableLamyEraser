#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include "orientation.h"
#include "actions.h"

void writeEvent(int fd, struct input_event event) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  event.time = tv;
  //printf("writing: seconds = %ld, usec= %ld, type = %d, code = %d, value = %d\n",
  //     event.time.tv_sec, event.time.tv_usec, event.type, event.code, event.value);
  write(fd, &event, sizeof(struct input_event));
}

void writeTapWithTouch(int fd_touch, const int location[]) {
  struct input_event event;

  // this is the minimum (probably) seqeunce of events that must be sent to tap
  // the screen in a location.
  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_SLOT, .value = 15}; // Use slot 15. Should be high enough, right?
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

int writeOrientedTapSequence(int fd_touch, toolbarOrientation *orientation, int RMversion, int numLocations, ...) {
  //give the fd to use, a pointer to a toolbarOrientation struct, the RMversion, and a list of action locations
  //returns 0 on success, -1 on failure
  int actionLocation[2];
  int action;
  va_list actionType;
  va_start (actionType, numLocations);
  printf("Orientation: %d\n", orientation->orientation);
  RMversion--; //RMversion is either 1 or 2, decrement it to be 0 or 1 to work with array.
  if(orientation->openNotebook) {
      for (int i = 0; i < numLocations; i++)  {
          action = va_arg(actionType, int);
          for(int j = 0; j < 2; j++)
              actionLocation[j] =  locationLookup[RMversion][action][orientation->orientation][j];
          printf("Resolved Location for RM%d: {%d,%d}\n", RMversion+1, actionLocation[0], actionLocation[1]);
          writeTapWithTouch(fd_touch, actionLocation);

        }
      va_end(actionType);
      return 0;
      }
  else {
    return -1; //no open notebook, didn't perform actions
    }
}
//writeOrientedTapSequence(fd_touch, &orientation, int RMversion, 4, UNDO, TOOLBAR, UNDO, TOOLBAR);


bool doublePressHandler(struct input_event ev_pen) {
  // returns true if a double press has happened
  static struct timeval prevTime;
  const double maxDoublePressTime = .5; // 500ms seems to be the standard double press time
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

void toggleModeRM2(struct input_event ev_pen, int fd_pen) {
  static int toggle = 0;
  if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) { // change state of toggle on button press
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

void pressModeRM2(struct input_event ev_pen, int fd_pen) {
  if (ev_pen.code == BTN_STYLUS) {
    ev_pen.code = BTN_TOOL_RUBBER; // value will follow the button, so we can reuse
                               // the message
    writeEvent(fd_pen, ev_pen);
  }
}

void toggleModeRM1(struct input_event ev_pen, int fd_touch, int RMversion) {
  static int toggle = 0;
  static int actionComplete = 0;
  static toolbarOrientation orientation;

  if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) { // change state of toggle on button press
    toggle = !toggle;
    orientation = getToolbarOrientation();
    actionComplete = 0;
    printf("toggle: %d \n", toggle);
  }

  if (toggle && !actionComplete) {
    printf("writing eraser tool...\n");
    writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, ERASER, TOOLBAR, ERASER, TOOLBAR);
    actionComplete = 1;
  } else if (!actionComplete) {
    printf("writing writing tool...\n");
    writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
    actionComplete = 1;
  }
}

void pressModeRM1(struct input_event ev_pen, int fd_touch, int RMversion) {
  static toolbarOrientation orientation;
  static int state = -1;
  if (ev_pen.code == BTN_STYLUS) {
    orientation = getToolbarOrientation();
    if(ev_pen.value == 1) {
        printf("writing eraser tool...\n");
        writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, ERASER, TOOLBAR, ERASER, TOOLBAR);
        state = 1;
      }
    else {
        printf("writing writing tool...\n");
        writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
        state = 0;
      }
    }
  else if (ev_pen.code == BTN_TOOL_PEN && ev_pen.value == 1 && state != -1) {
      if (state == 1) {
          printf("writing eraser tool...\n");
          writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, ERASER, TOOLBAR, ERASER, TOOLBAR);
        }
      else if (state == 0)
        {
          printf("writing writing tool...\n");
          writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
        }
  }
}

void actionToolbar(int fd_touch, int RMversion) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(fd_touch, &orientation, RMversion, 1, TOOLBAR);
}

void actionWriting(int fd_touch, int RMversion) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
}

void actionUndo(int fd_touch, int RMversion) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, UNDO, TOOLBAR, UNDO, TOOLBAR);
}

void actionRedo(int fd_touch, int RMversion) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(fd_touch, &orientation, RMversion, 4, REDO, TOOLBAR, REDO, TOOLBAR);
}



