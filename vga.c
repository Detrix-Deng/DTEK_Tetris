#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

const int WIDTH = 320;
const int HEIGHT = 240;

volatile char *VGA = (volatile char *)0x08000000;


void output(char array[HEIGHT][WIDTH]){

    for (int y = 0; y < WIDTH; y++)
    {
        for (int x = 0; x < HEIGHT; x++)
        {
            VGA[y * 320 + x] = array[y][x];
        }
    }

}