//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "shapes.c"
#include "vga.c"

//add GPIO pointer, timer pointer, VGA pointer

//s

<<<<<<< HEAD
// Global variables
=======
const int WIDTH = 320;
const int HEIGHT = 240;

>>>>>>> c5dd6dba789eb6079b217e88f2ef0e6e5b4a0ea7
int score;
char hold;      // Stores the type_id of the hold sprite
char grid[HEIGHT][WIDTH];
struct sprite curr_sprite;
struct sprite next_sprite[3];  // list containing the 3 upcoming sprite.

void render(){      // Renders the gamescreen
// Is called by the interrupt handler, to update and render the gamescreen.

}

int get_rand(){
    // take the snapL of timer, and then do some calculation to generate a random int.

}

bool collision_detect(){    // Check if the space below the sprite is occupied.
    // If occupied, return True, else, return False
    bool collision = false;

    return collision;
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

void score_calc(int layers){  // Calculates/update game score

}

int line_clear(){   // After collision detect == True
    // Check if any relevant layer is full.
    // If layer is full, layer_cleared++
    // move everything in grid above the lowest cleared
    // layer by layer_cleared amount
    int layer_cleared = 0;
    return 0; // Temporary! 
}

void mov_down(){    // x in curr_shape -= 1
    // This is polled every game cycle
    // Call line_clear if collision_check
    // If line_clear > 0, call score_calc

}

void mov_hor(unsigned int input){   // y in curr_shape +- 1, depending on input
    // Check whether input is from P1 or P2
    // Check whether input correspond to left or right

}

void loop(){    // game loop
    // poll inputs
}