#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

#define PEN_DEVICE      "/dev/input/event1"
#define PRESS_MODE      1 //press and hold mode
#define TOGGLE_MODE     2 //toggle mode


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

bool doublePressHandler(struct input_event ev) {
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

int main(int argc, char *argv[]) {
    int mode;
    struct input_event ev_pen;
    int fd_pen;

    char name[256] = "Unknown";

    if (argc > 2) {
        printf("Too many arguments supplied.");
        exit(EXIT_FAILURE);
      }
    else if (argc == 2) {
        if (!strncmp(argv[1], "--toggle", 8)) {
          printf("MODE: TOGGLE\n");
          mode = TOGGLE_MODE;
        }
        else if (!strncmp(argv[1], "--press", 7)) {
            printf("MODE: PRESS AND HOLD\n");
            mode = PRESS_MODE;
          }
          else {
            printf("Unknown argument %s\nExiting...\n", argv[1]);
            exit(EXIT_FAILURE);
          }
      }
     else {
        printf("No arguments supplied. Using Default Mode\n");
        printf("MODE: PRESS AND HOLD\n");
        mode = PRESS_MODE;
        }

    if ((getuid()) != 0) {
        fprintf(stderr, "You are not root! This may not work...\n");
        return EXIT_SUCCESS;
    }

    /* Open Device */
    fd_pen = open(PEN_DEVICE, O_RDWR);
    if (fd_pen == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE);
        exit(EXIT_FAILURE);
      }


    /* Print Device Name */
    ioctl(fd_pen, EVIOCGNAME(sizeof(name)), name);
    printf("Reading from:\n");
    printf("device file = %s\n", PEN_DEVICE);
    printf("device name = %s\n", name);


    for (;;) {
        //struct timeval tv;
        //gettimeofday(&tv, NULL);
        //printf("Time: %ld.%ld\n", tv.tv_sec, tv.tv_usec);

        const size_t ev_pen_size = sizeof(struct input_event);
        read(fd_pen, &ev_pen, ev_pen_size); //note: read pauses until there is data

        if(doublePressHandler(ev_pen)) {
          printf("double click detected\n");
          //do double click action
          }

        switch(mode) {
          case TOGGLE_MODE :
            toggleMode(ev_pen, fd_pen);
            break;
          case PRESS_MODE :
            pressMode(ev_pen, fd_pen);
            break;
          default :
            printf("Somehow a mode wasn't set?\n");
            exit(EXIT_FAILURE);
          }
      }
    return EXIT_SUCCESS;
}
