#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>

#include "orientation.h"
#include "triggers.h"
#include "effects.h"


void writeEvent(int fd, struct input_event event) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  event.time = tv;
  //printf("writing: seconds = %ld, usec= %ld, type = %d, code = %d, value = %d\n",
  //     event.time.tv_sec, event.time.tv_usec, event.type, event.code, event.value);
  write(fd, &event, sizeof(struct input_event));
}

static int currentTrackingID;
void handleCurrentTrackingID(struct input_event* event)  {
  if (event->type == EV_ABS && event->code == ABS_MT_TRACKING_ID) {
    if(event->value != -1) {
        currentTrackingID = event->value;
        //printf("Grabbing tracking ID: %d\n", currentTrackingID);
      }
    }
}


void writeTapWithTouch(int fd_touch, const int location[2]) {
  struct input_event event;

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1};
  // printf("Writing SYN Report\n");
  writeEvent(fd_touch, event);

  // this is the minimum (probably) seqeunce of events that must be sent to tap
  // the screen in a location.
  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_SLOT, .value = 15}; // Use slot 15. Should be high enough, right?
  // printf("Writing ABS_MT_SLOT: %d\n", event.value);
  writeEvent(fd_touch, event);

  event = (struct input_event){.type = EV_ABS, .code = ABS_MT_TRACKING_ID, .value = ++currentTrackingID};
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

void writeTapWithWacom(int fd_wacom, const int location[2]) {
  struct input_event event;
  struct timeval tv;

  // this is the minimum (probably) seqeunce of events that must be sent to tap
  // the screen in a location.
  // SYN
  // touch off
  // new x and y
  // touch on
  // touch off
  // SYN
  gettimeofday(&tv, NULL);
  event.time = tv;

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1}; //SYN
  //printf("Writing SYN Report\n");
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_KEY, .code = BTN_TOUCH, .value = 0}; //TOUCH OFF
  //printf("Writing BTN_TOUCH: %d\n", event.value);
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1}; //SYN
  //printf("Writing SYN Report\n");
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_ABS, .code = ABS_X, .value = location[0]}; //X
  //printf("Writing Pen X: %d\n", event.value);
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_ABS, .code = ABS_Y, .value = location[1]}; //Y
  //printf("Writing Pen Y: %d\n", event.value);
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_KEY, .code = BTN_TOUCH, .value = 1}; //TOUCH ON
  //printf("Writing BTN_TOUCH: %d\n", event.value);
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1}; //SYN
  //printf("Writing SYN Report\n");
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_KEY, .code = BTN_TOUCH, .value = 0}; //TOUCH OFF
  //printf("Writing BTN_TOUCH: %d\n", event.value);
  write(fd_wacom, &event, sizeof(struct input_event));

  event = (struct input_event){.type = EV_SYN, .code = SYN_REPORT, .value = 1}; //SYN
  //printf("Writing SYN Report\n");
  write(fd_wacom, &event, sizeof(struct input_event));
}

int writeOrientedTapSequence(int device, int fd, toolbarOrientation *orientation, int numLocations, ...) {
  //give the fd to use, a pointer to a toolbarOrientation struct, the RMversion, and a list of action locations
  //returns 0 on success, -1 on failure
  int actionLocation[2];
  int action;
  va_list actionType;
  va_start (actionType, numLocations);
  //printf("Orientation: %d | ", orientation->orientation);
  if(orientation->openNotebook) {
      if(device == WACOM) {
          printf("wacom digitizer: ");
          for (int i = 0; i < numLocations; i++)  {
              action = va_arg(actionType, int);
              actionLocation[0] = locationLookupWacom[action][orientation->orientation][0];
              actionLocation[1] = locationLookupWacom[action][orientation->orientation][1];
              //printf("{%d,%d} ", actionLocation[0], actionLocation[1]);
              writeTapWithWacom(fd, actionLocation);
            }
        }
      else { //Touch
        //printf("touch screen: ");
        for (int i = 0; i < numLocations; i++)  {
            action = va_arg(actionType, int);
            actionLocation[0] = locationLookupTouch[orientation->docType][action][orientation->orientation][0];
            actionLocation[1] = locationLookupTouch[orientation->docType][action][orientation->orientation][1];
            //printf("{%d,%d} ", actionLocation[0], actionLocation[1]);
            writeTapWithTouch(fd, actionLocation);
         }
       }
      //printf("\n");
      va_end(actionType);
      return 0;
      }
  else {
    return -1; //no open notebook, didn't perform actions
    }
}

/*-----------------------------------------------------------------
 * Single shot actions: compatible with clicks
 * -----------------------------------------------------------------*/

void actionToolbar(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 1, TOOLBAR);
}

void actionWriting(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
}

void actionText (int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 3, TEXT, TOOLBAR, TEXT);
}

void actionEraserPanel(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 3, ERASER_PANEL, TOOLBAR, ERASER_PANEL);
}

void actionUndo(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 4, UNDO, TOOLBAR, UNDO, TOOLBAR);
}

void actionRedo(int fd_touch) {
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 4, REDO, TOOLBAR, REDO, TOOLBAR);
}


/*-----------------------------------------------------------------
 * State-type / Continous actions
 * compatible with press and hold, and with clicks as toggles.
 * -----------------------------------------------------------------*/

static int ToolEraserRM2 = 0;
void activateToolEraserRM2(int fd_pen) {
  printf("Activating ToolEraserRM2: writing eraser on\n");
  writeEvent(fd_pen, tool_rubber_off);
  writeEvent(fd_pen, tool_rubber_on);
  ToolEraserRM2 = 1;
}
void deactivateToolEraserRM2(int fd_pen) {
  printf("Deactivating ToolEraserRM2: writing pen on\n");
  writeEvent(fd_pen, tool_pen_off);
  writeEvent(fd_pen, tool_pen_on);
  ToolEraserRM2 = 0;
}
void toggleToolEraserRM2(int fd_pen) {
  if (ToolEraserRM2)
    deactivateToolEraserRM2(fd_pen);
  else
    activateToolEraserRM2(fd_pen);
}
void actionToolEraserRM2(struct input_event* ev_pen, int fd_pen) {
  if (ToolEraserRM2) {
      if (ev_pen->code == BTN_TOOL_PEN && ev_pen->value == 1) { // while activated, we write rubber_on or off to supercede any TOOL_PEN messages
          printf("From actionToolEraserRM2: writing eraser on\n");
          writeEvent(fd_pen, tool_rubber_off);
          writeEvent(fd_pen, tool_rubber_on);
          }
    }
}

static int toolEraseSelect = 0;
void activateToolEraserSelect(int fd_touch) {
  //printf("Activating ToolEraseSelect: writing erase_select tool on\n");
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 8, ERASER_PANEL, ERASER_PANEL, ERASER_SELECTION, TOOLBAR, ERASER_PANEL, ERASER_PANEL, ERASER_SELECTION, TOOLBAR);
  toolEraseSelect = 1;
}
void deactivateToolEraserSelect(int fd_touch) {
  //printf("Deactivating ToolEraseSelect: writing writing_tool on\n");
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
  toolEraseSelect = 0;
}
void toggleToolEraserSelect(int fd_touch) {
  if (toolEraseSelect)
    deactivateToolEraserSelect(fd_touch);
  else
    activateToolEraserSelect(fd_touch);
}

static int toolSelect = 0;
void activateToolSelect(int fd_touch) {
  //printf("Activating ToolSelect: writing select tool on\n");
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 4, SELECT, TOOLBAR, SELECT, TOOLBAR);
  toolSelect = 1;
}
void deactivateToolSelect(int fd_touch){
  //printf("Deactivating ToolSelect: writing writing tool on\n");
  toolbarOrientation orientation = getToolbarOrientation();
  writeOrientedTapSequence(TOUCH, fd_touch, &orientation, 4, WRITING, TOOLBAR, WRITING, TOOLBAR);
  toolSelect = 0;
}
void toggleToolSelect(int fd_touch) {
  if (toolSelect)
    deactivateToolSelect(fd_touch);
  else
    activateToolSelect(fd_touch);
}

//test stored locations in effects_data.h
void testLocations(int device, int fd) {
  const char* str[4];
  str[0] = "Right Hand Portrait";
  str[1] = "Right Hand Landscape";
  str[2] = "Left Hand Portrait";
  str[3] = "Left Hand Landscape";

  for(int i = 0; i < 4; i++) {
      printf("Please set notebook to %s orientation. Close the tool bar and draw a large circle.\n"
             "When ready, press enter\n", str[i]);
      getchar(); //pauses until user presses enter


      toolbarOrientation orientation = getToolbarOrientation();
      printf("opening Toolbar...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, TOOLBAR);
      getchar();

      printf("tapping writing utensil...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, WRITING);
      getchar();

      printf("tapping eraser panel...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, ERASER_PANEL);
      getchar();

      printf("tapping eraser panel...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, ERASER_PANEL);
      getchar();

      printf("tapping erase selection...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, ERASER_SELECTION);
      getchar();

      printf("tapping eraser...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, ERASER_ERASE);
      getchar();

      printf("tapping select tool...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, SELECT);
      getchar();

      printf("tapping undo...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, UNDO);
      getchar();

      printf("tapping redo...\n");
      writeOrientedTapSequence(device, fd, &orientation, 1, REDO);

      printf("Completed %s.\n", str[i]);
    }
  printf("Completed all orientations.\n");
  exit(EXIT_SUCCESS);
}
