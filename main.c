//main stuff

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "tetris.c"

// labinit from lab 3 with different period values
void labinit() // Clock times out (TO) every 10/3 ms
{
    volatile int *time_addr = (volatile int *)0x04000020;
    *(time_addr + 2) = (100000 - 1) & 0x0000FFFF; // Set lower half
    *(time_addr + 3) = (100000 - 1) >> 16;        // Set upper half
    *(time_addr + 1) = 7;
    enable_interrupt();
}

void main(){
    //setup board things, enable interrupt, etc
    labinit();

    // Call main game loop in tetris.c
    loop();
}