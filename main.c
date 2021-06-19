#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

#define PEN_DEVICE      "/dev/input/event1"

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

int main(int argc, char *argv[]) {
    struct input_event ev_pen;
    int fd_pen;
    bool toggleMode = false;
    bool toggle = 0;
    char name[256] = "Unknown";

    if (argc > 2) {
        printf("Too many arguments supplied.");
        return EXIT_FAILURE;
      }
    else if (argc == 2) {
        if (!strncmp(argv[1], "--toggle", 8)) {
        printf("MODE: TOGGLE\n");
        toggleMode = true;
        }
      }
    else {
      printf("MODE: PRESS AND HOLD\n");
      toggleMode = false;
      }

    if ((getuid()) != 0) {
        fprintf(stderr, "You are not root! This may not work...\n");
        return EXIT_SUCCESS;
    }

    /* Open Device */
    fd_pen = open(PEN_DEVICE, O_RDWR);
    if (fd_pen == -1) {
        fprintf(stderr, "%s is not a vaild device\n", PEN_DEVICE);
        return EXIT_FAILURE;
      }


    /* Print Device Name */
    ioctl(fd_pen, EVIOCGNAME(sizeof(name)), name);
    printf("Reading from:\n");
    printf("device file = %s\n", PEN_DEVICE);
    printf("device name = %s\n", name);


    for (;;) {
        const size_t ev_pen_size = sizeof(struct input_event);
        read(fd_pen, &ev_pen, ev_pen_size);

        if (toggleMode) { //toggle mode
            if (ev_pen.code == BTN_STYLUS && ev_pen.value == 1) {
                toggle = !toggle;
                printf("toggle: %d\n", toggle);
              }
            if (toggle)
              if (ev_pen.code == BTN_TOOL_PEN) {
                  if (ev_pen.value == 1) {
                    printf("writing eraser on");
                    writeEvent(fd_pen, tool_rubber_on);
                    }
                  else {
                      printf("writing eraser off");
                      writeEvent(fd_pen, tool_rubber_off);
                  }
            }
          }
        else //press and hold mode
          {
            if (ev_pen.code == BTN_STYLUS) {
                ev_pen.code = BTN_TOOL_RUBBER; //value will follow the button, so we can reuse the message
                writeEvent(fd_pen, ev_pen);
              }
          }
      }
    return EXIT_SUCCESS;
}
