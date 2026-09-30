//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "shapes.c"
#include "vga.c"
#include "main.c"

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
int to_count = 0;   // Counter for TO flags
int difficulty = 30;    // Determines how fast mov_down is called

void render(){      // Renders the gamescreen
// Is called by the interrupt handler, to update and render the gamescreen.
    char vga_buffer = VGA;


}

unsigned int get_rand(){
    // take the snapL of timer, and then do some calculation to generate a random int.
    volatile int *time_addr = (volatile int *)0x04000020;
    time_addr += 4; //snapL
    unsigned int rand = *time_addr;
    return rand;
}

bool collision_detect(){    // Check if the space below the sprite is occupied.
    // If occupied, return True, else, return False
    bool collision = false;

    return collision;
}

void spawn_sprite(int player, int index){     // Update curr_sprite with next_sprite
    int rand_int = get_rand() % 6;
    struct sprite_shape shape = sprite_shapes[rand_int];
    next_sprite[player][index].sprite_shape = shape;
    next_sprite[player][index].x = 0;   // CHANGE!! (player * field) + offset;
    next_sprite[player][index].y = 0; // CHANGE!! offset;
}

void get_next_sprite(int player){
    curr_sprite[player] = next_sprite[player][0];
    next_sprite[player][0] = next_sprite[player][1];
    next_sprite[player][1] = next_sprite[player][2];
    spawn_sprite(player, 2);
}

void score_calc(int players){  // Calculates/update game score
    int mult = players * 1000;
    score[players] += mult; //add switch cases later
}

int line_clear(){   // After collision detect == True
    // Check if any relevant layer is full.
    // If layer is full, layer_cleared++
    // move everything in grid above the lowest cleared
    // layer by layer_cleared amount
    int layer_cleared = 0;
    return 0; // Temporary! 
}

void rotate(int player)
{
    // rotate curr_sprite[player] right or left based on direction
    unsigned char new_shape[4] = {0};
    // Rotate and save shape as an array of bits
    for(int i = 0; i < 4; i++)
    {
        for(int j = 3; j >= 0; j--)
            new_shape[i] = new_shape[i] | ((curr_sprite[player].sprite_shape.matrix[j][i]) << (3 - j));
    }
    // Shift left
    int min_dist = 3;
    for(int i = 0; i < 4; i++)
    {
        int dist = 0;
        if(new_shape[i] == 0)
            continue;
        for(int j = 3; j >= 0; j--)
        {
            if((new_shape[i] >> j) & 0x01)
                break;
            dist++;
        }
        if(dist < min_dist)
            min_dist = dist;
    }
    // Translate and save new_shape to curr_sprite
    for(int i = 0; i < 4; i++)
        new_shape[i] = new_shape[i] << min_dist;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 3; j >= 0; j--)
            curr_sprite[player].sprite_shape.matrix[i][3 - j] = (new_shape[i] >> j) & 0x01;
    }
}

void mov_down(int player){    // y in curr_shape -= 1
    // This is polled every game cycle
    // Call line_clear if collision_check
    // If line_clear > 0, call score_calc
    if(collision_detect){

    }else{
        curr_sprite[player].y += -1;
    }
}

void hard_down(int player){
    // move sprite all the way down
}

void mov_hor(int player, int direction){   // x in curr_shape +- 1, depending on direction
    curr_sprite[player].x += direction;
    // Add limit checks to confirm sprite in border
}

void hold_func(int player){
    hold[player] = curr_sprite[player].sprite_shape.sprite_id;
    spawn_sprite(player);
}

void interrupt_handler(unsigned int cause){
    volatile int *time_addr = (volatile int *)0x04000020;
    *time_addr = 2; // Clear TO flag
    render();
    to_count++;
    if(to_count >= difficulty)
    {
        to_count = 0;
        if(multiplayer)
            mov_down(2);
        mov_down(1);
    }
}

void loop(){    // game loop
    // poll inputs
    int rot1 = *gpio & 0x0001;
    int mv_r1 = *gpio & 0x0002;
    int mv_l1 = *gpio & 0x0004;
    int down1 = *gpio & 0x0008;
    int hold1 = *gpio & 0x0010;
    int rot2 = *gpio & 0x0020;
    int mv_r2 = *gpio & 0x0040;
    int mv_l2 = *gpio & 0x0080;
    int down2 = *gpio & 0x0100;
    int hold2 = *gpio & 0x0200;

    // Player 1
    if (rot1){
        rotate(1);
    }
    if(mv_r1){
        mov_hor(1, 1);
    }
    if(mv_l1){
        mov_hor(1, -1);
    }
    if(down1){
        hard_down(1);
    }
    if(hold1){
        hold_func(1);
    }

    // Player 2
    if(multiplayer)
    {
        if (rot2){
            rotate(2);
        }
        if(mv_r2){
            mov_hor(2, 1);
        }
        if(mv_l2){
            mov_hor(2, -1);
        }
        if(down2){
            hard_down(2);
        }
        if(hold2){
            hold_func(2);
        }
    }
}