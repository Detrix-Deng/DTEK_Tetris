//main stuff

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "tetris.c"

bool multiplayer = false;
bool start = false;

// labinit from lab 3 with different period values
void labinit() // Clock times out (TO) every 10/3 ms
{
    volatile int *time_addr = (volatile int *)0x04000020;
    *(time_addr + 2) = (100000 - 1) & 0x0000FFFF; // Set lower half
    *(time_addr + 3) = (100000 - 1) >> 16;        // Set upper half
    *(time_addr + 1) = 7;
    enable_interrupt();
}

void mmanu(){
    char text = "TETRIS";
    volatile int *gpio = (volatile int *) 0x040000e0;
    if (*gpio == 1){
        multiplayer = false;
        start = true;
    } else if (*gpio == 2){
        multiplayer = true;
        start = true;
    }
}

void main(){
    //setup board things, enable interrupt, etc
    labinit();

    while(!start){
        mmanu();
    }

    set_offset(multiplayer);
    // Call main game loop in tetris.c
    while(1){
        loop();
    }
}