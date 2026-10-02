#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "vga_text.c"
#include "shapes.c"
#ifndef VGA_H
#define VGA_H

#define WIDTH 320
#define HEIGHT 240
#define GRID_WIDTH 10
#define GRID_HEIGHT 20

volatile char *VGA_addr = (volatile char *)0x08000000;

int old_offset[2][2] = { //x,y
    {0,0},
    {0,0}
};

void put_grid(char array[HEIGHT][WIDTH], char grid[GRID_HEIGHT][GRID_WIDTH], int x_offset, int y_offset, int id){
    int x = x_offset;
    int y = y_offset;
    int player = id; //unused but kept for consistency with other functions

    //loop for terinions
    for(int row = 0; row < GRID_HEIGHT; row++){
        for(int col = 0; col < GRID_WIDTH; col++){
            if(grid[row][col]){
                array[y + row][x + col] = 255;
            } else {
                array[y + row][x + col] = 0;
            }
        }
    }

    //loop for border
    for(int vert = 0; vert < GRID_HEIGHT + 2; vert++){
        array[y - 1 + vert][x - 1] = 255;
        array[y - 1 + vert][x + GRID_WIDTH + 1] = 255;
    } 
    for(int hor = 0; hor < GRID_WIDTH + 2; hor++){
        array[y - 1][x - 1 + hor] = 255;
        array[y + GRID_HEIGHT + 1][x - 1 + hor] = 255;
    }
}

//Draw sprite onto VGA buffer
void put_sprite(char array[HEIGHT][WIDTH], char sprite[4][4], int x_offset, int y_offset, bool is_curr, int id){
    char (*matrix)[4] = sprite;
    int x = x_offset;
    int y = y_offset - 3; //offset for sprites

    if(is_curr){    //if is a curr_sprite, remove last sprite pos and update last pos with new sprite.
        int player = id;
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                if(matrix[row][col]){
                    array[row + old_offset[player][1] - 3][col + old_offset[player][0]] = 0;
                }
            }
        }
        old_offset[player][0] = x;
        old_offset[player][1] = y + 3;
    }
    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            if(matrix[row][col]){
                array[row + y][col + x] = 255;
            } else {
                array[row + y][col + x] = 0;
            }
        }
    }
}

//Feed in matrix, text, offset positions
void put_text(char array[HEIGHT][WIDTH], char *text, int x_offset, int y_offset, int scalar){
    int x = x_offset;
    int y = y_offset;
    int i = 0;
      //calculate buffer array to copy font table into
    while(text[i] != '\0'){             //gets the text from char array
        char c = text[i];
        int font_int = 0; //font_int is the index for the alphanum array
        if (c <= 57 && c >= 48){        //if numbers
            font_int = c - 48;
        } else if (c <= 90 && c >= 65){ //if letters        
            font_int = c - 55;
        } else if (c == 32){
            font_int = 36;
        } else if (c == 58){
            font_int = 37;
        }

        // Font rows
        for (int row = 0; row < 7; row++){
            // Font columns
            for (int col = 0; col < 5; col++){
                if (alphanum[font_int][row] & (1 << (4 - col))){
                    for(int dy = 0; dy < scalar; dy++){
                        for(int dx = 0; dx < scalar; dx++){
                            array[y + row + dy][x + i * 6 * scalar + col + dx] = 255;
                        }
                    }
                } else {
                    for(int dy = 0; dy < scalar; dy++){
                        for(int dx = 0; dx < scalar; dx++){
                            array[y + row + dy][x + i * 6 * scalar + col + dx] = 0;
                        }
                    }
                }
            }
        }
    }
}

void render(char array[HEIGHT][WIDTH]){
    volatile char *VGA_addr = (volatile char *)0x08000000;
    console_out(array);
    for (int y = 0; y < WIDTH; y++){
        for (int x = 0; x < HEIGHT; x++){
            VGA_addr[y * 320 + x] = array[y][x];
        }
    }

}

void clear_display(char array[HEIGHT][WIDTH]){
    for(int row = 0; row < HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++){
            array[row][col] = 0;
            render(array);
        }
    }
}

#endif