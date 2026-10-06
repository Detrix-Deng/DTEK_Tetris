//main stuff

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dtekv-lib.h"
#include "tetris.h"
#include "vga.h"
#include "shapes.h"
#include "config.h"


extern void enable_interrupt();
extern void delay(int ms);




bool start = false;
bool test = true; //for testing purposes, set to true to skip menu and go straight to test screen

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
    //TODO: Implement call to draw text on screen
    put_text(VGA, "TETRIS", 85, 20, 5);
    put_text(VGA, "SINGLEPLAYER", 20, 100, 1);
    put_text(VGA, "MULTIPLAYER", 240, 100, 1);

    int value = read_gpio();
    
    if (value == 0x02){
        // print("1 is pressed");
        multiplayer = false;
        start = true;
    } else if (value == 0x08){
        // print("2 is pressed");
        multiplayer = true;
        start = true;
    } else
        // print("Not pressed");

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

    put_text(VGA, "SCORE: 00000", 0,0,1);
    put_text(VGA, "HIJKLMNOP",0,20,1);
    put_text(VGA, "QRSTUVWXYZ",0,40,1);
    put_text(VGA,"0123456789",0,60,1);
    put_text(VGA, ": space space", 0, 80, 1);
    render(VGA);
    //setup board things, enable interrupt, etc
    /* player_init(false);
    int player = 0;
    set_offset(false);
    put_grid(VGA, player_list[player].grid, OFFSET_X[player], OFFSET_Y, 3);

    // Draw Hold and Next Grid manually
    // Hold grid
    put_line(VGA, OFFSET_X[player] - 3 - (2 * 4) - 2, OFFSET_Y - 3, (2 * 4) + 2, 2);
    put_line(VGA, OFFSET_X[player] - 3 - (2 * 4) - (2 * 2), OFFSET_Y - 3, 2, (2 * 4) + (2 * 2) + 2);
    put_line(VGA, OFFSET_X[player] - 3 - (2 * 4) - 2, OFFSET_Y + (2 * 4) + 1, (2 * 4) + 2, 2);

    // Next grid
    put_line(VGA, OFFSET_X[player] + (3 * GRID_WIDTH) + 3, OFFSET_Y - 3, (2 * 4) + 2, 2);
    put_line(VGA, OFFSET_X[player] + (3 * GRID_WIDTH) + 3 + (2 * 4) + 2, OFFSET_Y - 3, 2, (3 * 2 * 4) + (4 * 1) + (2 * 2));
    put_line(VGA, OFFSET_X[player] + (3 * GRID_WIDTH) + 3, OFFSET_Y + (3 * 2 * 4) + (3 * 1), (2 * 4) + 2, 2);
    render(VGA);*/
    }

}