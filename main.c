//main stuff

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dtekv-lib.h"
#include "tetris.h"
#include "vga.h"
#include "shapes.h"
#include "config.h"
#include "performance.h"

extern void enable_interrupt();
extern void delay(int ms);

bool start = false;
bool test = false; //for testing purposes, set to true to skip menu and go straight to test screen

// Contributed by Both
// labinit from lab 3 with different period values
void labinit() // Clock times out (TO) every 10/3 ms
{
    volatile int *time_addr = (volatile int *)0x04000020;
    *(time_addr + 2) = (1000000 - 1) & 0x0000FFFF; // Set lower half
    *(time_addr + 3) = (1000000 - 1) >> 16;        // Set upper half
    *(time_addr + 1) = 7;
    enable_interrupt();
}

//By Ye
//Create a menu for multiplayer or single player selection
void mmanu(){
    //TODO: Implement call to draw text on screen
    put_text(VGA, "TETRIS", 75, 20, 5);
    put_text(VGA, "PRESS A", 35, 100, 1);
    put_text(VGA, "PRESS D", 230, 100, 1);
    put_text(VGA, "SINGLEPLAYER", 20, 140, 1);
    put_text(VGA, "MULTIPLAYER", 220, 140, 1);

    int value = *gpio;
    
    if (!(value & 0x04)){
        // print("1 is pressed");
        multiplayer = false;
        start = true;
    } else if (!(value & 0x02)){
        // print("2 is pressed");
        multiplayer = true;
        start = true;
    } else
        // print("Not pressed");

    render(VGA);
}

// Contributed by Dave
void performance_test(int n){
    // Used for performance testing a function
    struct performance_value performance;
    print("Begin performance test...\n");
    print("Running render(VGA); ");
    print_dec(n);
    print(" times...\n\n");

    // Reset all hardware counter registers to 0
    start_performance();

    for(int i = 0; i < n; i++){
        render(VGA);
    }

    stop_performance(&performance);

    // Print raw data
    print("=======TEST RESULTS=======\n\n");
    print("Number of clock cycles that elapsed (mcycle):\n");
    print_dec(performance.mcycleh);
    print_dec(performance.mcycle);
    print("\n\n");

    print("Number of instructions that have been retired (minstret):\n");
    print_dec(performance.minstreth);
    print_dec(performance.minstret);
    print("\n\n");

    print("Number of memory instructions that have been retired (mhpmcounter3):\n");
    print_dec(performance.mhpmcounter3h);
    print_dec(performance.mhpmcounter3);
    print("\n\n");

    print("Number of times an instruction-fetch resulted in a I-cache miss (mhpmcounter4):\n");
    print_dec(performance.mhpmcounter4h);
    print_dec(performance.mhpmcounter4);
    print("\n\n");

    print("Number of times an memory operation resulted in a D-cache (mhpmcounter5):\n");
    print_dec(performance.mhpmcounter5h);
    print_dec(performance.mhpmcounter5);
    print("\n\n");

    print("Number of stalls the CPU experienced due to I-cache misses (mhpmcounter6):\n");
    print_dec(performance.mhpmcounter6h);
    print_dec(performance.mhpmcounter6);
    print("\n\n");

    print("Number of stalls the CPU experienced due to D-cache misses (mhpmcounter7):\n");
    print_dec(performance.mhpmcounter7h);
    print_dec(performance.mhpmcounter7);
    print("\n\n");

    print("Number of stalls that the CPU experienced due to data hazards\n");
    print("that could not be solved by forwarding (mhpmcounter8):\n");
    print_dec(performance.mhpmcounter8h);
    print_dec(performance.mhpmcounter8);
    print("\n\n");
    
    print("Number of stalls that the CPU experienced due to expensive ALU operations (mhpmcounter9):\n");
    print_dec(performance.mhpmcounter9h);
    print_dec(performance.mhpmcounter9);
    print("\n\n");
    
    print("=======END OF TEST=======");
}

//By Both
void main(){
    //setup before starting the game, options, etc
    //performance_test(30);
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
        int i = 0;
        put_line(VGA, 0, 0, 100, 100);
        while(1){
            put_sprite(VGA, sprite_shapes[0].matrix, 1, i, true, 0, 1);
            render(VGA);
            i++;
        }
    }

}