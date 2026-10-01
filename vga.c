#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#define WIDTH = 320;
#define HEIGHT = 240;

volatile char *VGA_addr = (volatile char *)0x08000000;

//Feed in matrix, text, offset positions
void put_text(char array[HEIGHT][WIDTH], char *text, int x, int y, int scalar){
    int i = 0;
    int length = strlen(text); //calculate buffer array to copy font table into
    while(text[i] != '\0'){     //gets the text from char array
        char c = text[i];
        if (c <= 57 && c >= 48) //if numbers

        else if (c <= 90 && c >= 65)    //if letters
        {

        }
    }

    //nested loops to copy alphanum_buffer into matrix array

    for()

}

void output(char array[HEIGHT][WIDTH]){

    for (int y = 0; y < WIDTH; y++)
    {
        for (int x = 0; x < HEIGHT; x++)
        {
            VGA_addr[y * 320 + x] = array[y][x];
        }
    }

}