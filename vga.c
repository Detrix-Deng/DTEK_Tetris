#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "tetris.c"

volatile char *VGA = (volatile char *)0x08000000;

void output(char array[]){
    char array_to_send[WIDTH * HEIGHT];
    for(int i = 0; i<)
}