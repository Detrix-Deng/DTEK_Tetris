//main stuff

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dtekv-lib.h"
#include "tetris.h"
#include "vga.h"
#include "shapes.h"


extern void enable_interrupt();

bool start = false;
bool test = false; //for testing purposes, set to true to skip menu and go straight to test screen

//By Both
// labinit from lab 3 with different period values
void labinit() // Clock times out (TO) every 10/3 ms
{
    volatile int *time_addr = (volatile int *)0x04000020;
    *(time_addr + 2) = (100000 - 1) & 0x0000FFFF; // Set lower half
    *(time_addr + 3) = (100000 - 1) >> 16;        // Set upper half
    *(time_addr + 1) = 7;
    enable_interrupt();
}

//By Ye
//Create a menu for multiplayer or single player selection
void mmanu(){
    char *text = "TETRIS";
    //TODO: Implement call to draw text on screen
    put_text(VGA, text, 50, 20, 5);

    volatile int *gpio = (volatile int *) 0x040000e0;
    if (*gpio == 1){
        print("Is pressed");
        multiplayer = false;
        start = true;
    } else if (*gpio == 2){
        print("Not pressed");
        multiplayer = true;
        start = true;
    }
    render(VGA);
}

//By Both
void main(){
    //setup before starting the game, options, etc
    if(!test){
    while(!start){
        mmanu();
    }
    clear_display(VGA);

    set_offset(multiplayer);

    player_init(multiplayer);

    labinit();
    // Call main game loop in tetris.c
    while(1){
        loop();
    }
    } else {

    //setup board things, enable interrupt, etc
    put_text(VGA, "TETRIS", 0, 0, 5);
    put_sprite(VGA, sprite_shapes[0].matrix, 50, 50, false, 0, 1);
    put_grid(VGA, player_list[0].grid, 100, 100, 1);
    render(VGA);
    }

}