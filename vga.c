#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

const int WIDTH = 320;
const int HEIGHT = 240;

volatile char *VGA = (volatile char *)0x08000000;

char stringbuilder(char string[]){
    return string[];
}

void output(char array[]){
    char array_to_send[WIDTH * HEIGHT];
    for(int i = 0; i<WIDTH; i++){
        
    }
}