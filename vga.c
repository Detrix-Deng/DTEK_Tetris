#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#define WIDTH 320
#define HEIGHT 240

volatile char *VGA_addr = (volatile char *)0x08000000;

void put_sprite(char array[HEIGHT][WIDTH], struct sprite_shape sprite, int x, int y){
    char matrix = sprite.matrix;
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
                    array[y + row][x + col] = 255;
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