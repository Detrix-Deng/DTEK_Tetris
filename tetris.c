//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "shapes.c"
#include "vga.c"

//add GPIO pointer, timer pointer, VGA pointer

//s

const int WIDTH = 320;
const int HEIGHT = 240;

int score;
char hold;      // Stores the type_id of the hold sprite
char grid[HEIGHT][WIDTH];
struct sprite curr_sprite;
struct sprite next_sprite[3];  // list containing the 3 upcoming sprite.

void render(){      // Renders the gamescreen
// Is called by the interrupt handler, to update and render the gamescreen.
}

bool collision_detect(){        // Check if the space below the sprite is occupied.
    // If occupied, return True, else, return False

}

void spawn_sprite(int rand){     // Update curr_sprite with next_sprite

}

void interrupt_handler(unsigned int cause){
    switch (cause){ //interrupt from timer
        case 16:
            render();
            break;
        
        default:
            break;
    }
}

void score_calc();  // Calculates 

void loop(){    // game loop
    
}