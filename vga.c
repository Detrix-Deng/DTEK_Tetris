#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#define WIDTH = 320;
#define HEIGHT = 240;

volatile char *VGA = (volatile char *)0x08000000;

char put_text(char array[HEIGHT][WIDTH], char *text, int x, int y){
    int spacing = 5;
    int i = 0;
    int length = sizeof(text)/sizeof(char)
    char buffer[length][7];
    while(text[i] != '\0'){
        char c = text[i];
        if (c <= 57 && c >= 48)
            buffer[i] = alphanum[c - 48]
        else if (c <= 90 && c >= 65)
        {
            buffer[i] = alphanum[c - 55]
        }
    }

    

    return array;
}

void output(char array[HEIGHT][WIDTH]){

    for (int y = 0; y < WIDTH; y++)
    {
        for (int x = 0; x < HEIGHT; x++)
        {
            VGA[y * 320 + x] = array[y][x];
        }
    }

}