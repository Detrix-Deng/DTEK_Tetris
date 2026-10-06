//By Dave

#ifndef CONFIG_H
#define CONFIG_H

#define WIDTH 320
#define HEIGHT 240

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

extern volatile int *gpio = (volatile int *) 0x040000e0;
extern volatile int *direction = (volatile int *) 0x040000e4;

#endif