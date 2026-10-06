#include "config.h"

volatile int *gpio = (volatile int *) 0x040000e0;
volatile int *direction = (volatile int *) 0x040000e4;
volatile int *outclear = (volatile int *) 0x040000e0 + 5;
