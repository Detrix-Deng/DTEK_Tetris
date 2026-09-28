//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "shapes.c"
#include "vga.c"

//add GPIO pointer, timer pointer, VGA pointer
volatile int *gpio = (volatile int *) 0x040000e0;

//

const int WIDTH = 320;
const int HEIGHT = 240;
const int GRID_WIDTH = 8;
const int GRID_HEIGHT = 20;
char VGA[WIDTH * HEIGHT];  //vga buffer

int score;
char hold;      // Stores the type_id of the hold sprite
char grid[GRID_HEIGHT][GRID_WIDTH];
struct sprite curr_sprite[2];   // array with 2 struct of curr_sprite for each player
struct sprite next_sprite[2][3];  // 2 lists containing the 3 upcoming sprite.

void render(){      // Renders the gamescreen
// Is called by the interrupt handler, to update and render the gamescreen.
    char vga_buffer = VGA;


}

int get_rand(){
    // take the snapL of timer, and then do some calculation to generate a random int.
    volatile int *time_addr = (volatile int *)0x04000020;
    time_addr += 4; //snapL
    int rand = *time_addr;
    return rand;
}

bool collision_detect(){    // Check if the space below the sprite is occupied.
    // If occupied, return True, else, return False
    bool collision = false;

    return collision;
}

void spawn_sprite(int rand, int player){     // Update curr_sprite with next_sprite
    int rand_int = rand % 6;
    struct sprite_shape shape = sprite_shapes[rand_int];
    curr_sprite[player].sprite_shape = shape;
    curr_sprite[player].x = 0; // CHANGE!! (player * field) + offset;
    curr_sprite[player].y = 0; // CHANGE!! offset;
}

void interrupt_handler(unsigned int cause){
    render();
}

void score_calc(int layers){  // Calculates/update game score
    int mult = layers * 1000;
    score += mult; //add switch cases later
}

int line_clear(){   // After collision detect == True
    // Check if any relevant layer is full.
    // If layer is full, layer_cleared++
    // move everything in grid above the lowest cleared
    // layer by layer_cleared amount
    int layer_cleared = 0;
    return 0; // Temporary! 
}

void mov_down(){    // y in curr_shape -= 1
    // This is polled every game cycle
    // Call line_clear if collision_check
    // If line_clear > 0, call score_calc

}

void mov_hor(unsigned int input){   // x in curr_shape +- 1, depending on input
    // Check whether input is from P1 or P2
    // Check whether input correspond to left or right

}

void loop(){    // game loop
    // poll inputs
    int right = *gpio & 0x00000001;
    int left = *gpio & 0x0000002 ;
    int move_right = *gpio & 0x00000004;
    int move_left = *gpio & 0x00000008;
    int shift_down = *gpio & 0x00000010;
    int hold_shape = *gpio & 0x00000020;
    if (right){

    }
    if(left){

    }
    if(move_right){
        mov_hor(1);
    }
    if(move_left){
        mov_hor(1);
    }
    if(shift_down){
        mov_down();
    }
    if(hold){

    }

}