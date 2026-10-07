#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dtekv-lib.h"
#include "vga.h"
#include "shapes.h"

// By Ye
int old_offset[2][2] = { //x,y
    {0,0},
    {0,0}
};

// By Ye
char old_sprite[2][4][4] = {
    {{1,0,0,0}
    ,{1,0,0,0}
    ,{1,1,0,0}
    ,{0,0,0,0}},

    {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}
};

//By Ye
void put_grid(char array[HEIGHT][WIDTH], char grid[GRID_HEIGHT][GRID_WIDTH], int x_offset, int y_offset, int scalar){
    //Accepts VGA buffer, player grid, offset and scale to draw it on the VGA buffer
    int x = x_offset;
    int y = y_offset;

    //loop for terinions
    for(int row = 0; row < GRID_HEIGHT; row++){
        for(int col = 0; col < GRID_WIDTH; col++){
            if(grid[row][col]){
                for(int dy = 0; dy < scalar; dy++){
                    for(int dx = 0; dx < scalar; dx++){ //1 if pixel is occupied
                        array[y + row * scalar + dy][x + col * scalar + dx] = '1';
                    }
                }
            } else {
                for(int dy = 0; dy < scalar; dy++){
                    for(int dx = 0; dx < scalar; dx++){ //0 otherwise
                        array[y + row * scalar + dy][x + col * scalar + dx] = '0';
                    }
                }
            }
        }
    }

    //loop for border around grid
    for(int vert = 0; vert < GRID_HEIGHT + 2; vert++){
        for(int dy = 0; dy < scalar; dy++){
            for(int dx = 0; dx < scalar; dx++){
                array[y - 1 * scalar + vert * scalar + dy][x - 1 * scalar + dx] = '1';
                array[y - 1 * scalar + vert * scalar + dy][x + GRID_WIDTH * scalar + dx] = '1';
            }
        }
    } 
    for(int hor = 0; hor < GRID_WIDTH + 2; hor++){
        for(int dy = 0; dy < scalar; dy++){
            for(int dx = 0; dx < scalar; dx++){
                array[y - 1 * scalar + dy][x - 1 * scalar + hor * scalar + dx] = '1';
                array[y + GRID_HEIGHT * scalar + dy][x - 1 * scalar + hor * scalar + dx] = '1';
            }
        }
    }
}

//By Ye
//Draw sprite onto VGA buffer
void put_sprite(char array[HEIGHT][WIDTH], char sprite[4][4], int x_offset, int y_offset, bool is_curr, int id, int scalar){
    int x = x_offset;
    int y = y_offset; //offset for sprites
    int player = id;

    if(is_curr){    //if is a curr_sprite, remove last sprite via using old_sprite variable
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                if(old_sprite[player][row][col] == 1){
                    for(int dx = 0; dx < scalar; dx++){
                        for(int dy = 0; dy < scalar; dy++){
                            array[old_offset[player][1] + row * scalar + dy][old_offset[player][0] + col * scalar + dx] = '0';
                        }
                    }
                }
            }
        }
        old_offset[player][0] = x;
        old_offset[player][1] = y;
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                old_sprite[player][row][col] = sprite[row][col];
            }
        }
        //Place new sprite into VGA without overwrite
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                for(int dy = 0; dy < scalar; dy++){
                    for(int dx = 0; dx < scalar; dx++){
                        if(sprite[row][col]){
                            array[row * scalar + y + dy][col * scalar + x + dx] = '1';
                        }
                    }
                }
            }
        }
    } else {

        //Place new sprite into VGA with overwrite
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                for(int dy = 0; dy < scalar; dy++){
                    for(int dx = 0; dx < scalar; dx++){
                        if(sprite[row][col]){
                            array[row * scalar + y + dy][col * scalar + x + dx] = '1';
                        } else {
                            array[row * scalar + y + dy][col * scalar + x + dx] = '0';
                        }
                    }
                }
            }
        }
    }
}

//By Ye
//Feed in matrix, text, offset positions
void put_text(char array[HEIGHT][WIDTH], char *text, int x_offset, int y_offset, int scalar){
    int x = x_offset;
    int y = y_offset;
    int i = 0;
      //calculate buffer array to copy font table into
    while(text[i] != '\0'){             //gets the text from char array
        char c = text[i];
        int font_int = 0;               //font_int is the index for the alphanum array
        if (c <= 57 && c >= 48){        //if numbers
            font_int = c - 48;
        } else if (c <= 90 && c >= 65){ //if letters        
            font_int = c - 55;
        } else if (c == 32){            //special case
            font_int = 36;
        } else if (c == 58){            //special case
            font_int = 37;
        }

        // Font rows
        for (int row = 0; row < 7; row++){
            // Font columns
            for (int col = 0; col < 5; col++){
                if (alphanum[font_int][row] & (1 << (4 - col))){
                    for(int dy = 0; dy < scalar; dy++){
                        for(int dx = 0; dx < scalar; dx++){
                            array[y + row * scalar + dy][x + i * 6 * scalar + col * scalar + dx] = '1';
                        }
                    }
                } else {
                    for(int dy = 0; dy < scalar; dy++){
                        for(int dx = 0; dx < scalar; dx++){
                            array[y + row * scalar + dy][x + i * 6 * scalar + col * scalar + dx] = '0';
                        }
                    }
                }
            }
        }
        i++;
    }
}

//By Ye
void put_line(char array[HEIGHT][WIDTH], int x, int y, int length, int width){
    //creates a line
    for (int vert = 0; vert < width; vert++){
        for (int hori = 0; hori < length; hori++){
            array[vert + y][hori + x] = '1';
        }
    }
}

//By Ye
void render(char array[HEIGHT][WIDTH]){
    //copies VGA buffer content into the VGA pointer
        for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            if(array[y][x] == '1')
                print("1");
            else{
                print("O");
            }
        }
        print("\n");
    }
    volatile char *VGA_addr = (volatile char *)0x08000000;
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            if(array[y][x] == '1')
                VGA_addr[y * 320 + x] = 255;
            else{
                VGA_addr[y * 320 + x] = 0;
            }
        }
    } 
}

//By Ye
void clear_display(char array[HEIGHT][WIDTH]){
    //Clears display
    for(int row = 0; row < HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++){
            array[row][col] = '0';
        }
    }
}
