#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#define WIDTH 320
#define HEIGHT 240

volatile char *VGA_addr = (volatile char *)0x08000000;

int old_offset[2][2] = { //x,y
    {0,0},
    {0,0}
};

//Draw sprite onto VGA buffer
void put_sprite(char array[HEIGHT][WIDTH], struct sprite_shape sprite, int x, int y, bool is_curr, int id){
    char **matrix = sprite.matrix;
    int x = x;
    int y = y - 3; //offset for sprites

    if(is_curr){    //if is a curr_sprite, remove last sprite pos and update last pos with new sprite.
        int player = id;
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                if(matrix[row][col]){
                    array[row + old_offset[player][0]][col + old_offset[player][1] - 3] = 0;
                }
            }
        }
        old_offset[player][0] = x;
        old_offset[player][0] = y + 3;
    } else {    //else remove sprite at some location
        for(int row = 0; row < 4; row++){
            for(int col = 0; col < 4; col++){
                if(matrix[row][col]){
                    array[row + x][col + y] = 0;
                }
            }
        }
    }

    for(int row = 0; row < 4; row++){
        for(int col = 0; col < 4; col++){
            if(matrix[row][col]){
                array[row + x][col + y] = 255;
            }
        }
    }
}

//Feed in matrix, text, offset positions
void put_text(char array[HEIGHT][WIDTH], char *text, int x, int y, int scalar){
    int x = x;
    int y = y;
    int scalar = scalar;
    int i = 0;
    int length = strlen(text);          //calculate buffer array to copy font table into
    while(text[i] != '\0'){             //gets the text from char array
        char c = text[i];
        int font_int;
        if (c <= 57 && c >= 48){        //if numbers
            font_int = 'c' - 48;
        } else if (c <= 90 && c >= 65){ //if letters        
            font_int = 'c' - 55;
        } else if (c = 32){
            font_int = 36;
        }

        // Font rows
        for (int row = 0; row < 7; row++){
            // Font columns
            for (int col = 0; col < 5; col++){
                if (alphanum[font_int][row] & (1 << (4 - col))){
                    for(int dy = 0; dy < scalar; dy++){
                        for(int dx = 0; dx < scalar; dx++){
                            array[y + row + dy][x + col + dx] = 255;
                        }
                    }
                }
            }
        }
    }
}

void render(char array[HEIGHT][WIDTH]){

    for (int y = 0; y < WIDTH; y++)
    {
        for (int x = 0; x < HEIGHT; x++)
        {
            VGA_addr[y * 320 + x] = array[y][x];
        }
    }

}