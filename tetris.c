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

int score[2];
char hold[2];      // Stores the type_id of the hold sprite
char grid[2][GRID_HEIGHT][GRID_WIDTH];
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

void spawn_sprite(int player){     // Update curr_sprite with next_sprite
    int rand_int = get_rand() % 6;
    struct sprite_shape shape = sprite_shapes[rand_int];
    curr_sprite[player].sprite_shape = shape;
    curr_sprite[player].x = 0; // CHANGE!! (player * field) + offset;
    curr_sprite[player].y = 0; // CHANGE!! offset;
}

void interrupt_handler(unsigned int cause){
    volatile int *time_addr = (volatile int *)0x04000020;
    *time_addr = 2; // Clear TO flag
    render();
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

void rotate(int player, int direction)
{
    // rotate curr_sprite[player] right or left based on direction
}

void mov_down(){    // y in curr_shape -= 1
    // This is polled every game cycle
    // Call line_clear if collision_check
    // If line_clear > 0, call score_calc

}

void mov_hor(int player, int direction){   // x in curr_shape +- 1, depending on direction
    curr_sprite[player].x += direction;
    // Add limit checks to confirm sprite in border
}

void hold_func(int player){
    hold[player] = curr_sprite[player].sprite_shape.sprite_id;
    spawn_sprite(player);
}

void loop(){    // game loop
    // poll inputs
    int rot1 = *gpio & 0x01;
    int mv_r1 = (*gpio >> 1) & 0x01;
    int mv_l1 = (*gpio >> 2) & 0x01;
    int down1 = (*gpio >> 3) & 0x01;
    int hold1 = (*gpio >> 4) & 0x01;
    int rot2 = (*gpio >> 5) & 0x01;
    int mv_r2 = (*gpio >> 6) & 0x01;
    int mv_l2 = (*gpio >> 7) & 0x01;
    int down2 = (*gpio >> 8) & 0x01;
    int hold2 = (*gpio >> 9) & 0x01;
    if (rot1){

    }
    if(mv_r1){
        mov_hor(1, 1);
    }
    if(mv_l1){
        mov_hor(1, -1);
    }
    if(down1){
        mov_down();
    }
    if(hold1){

    }

}