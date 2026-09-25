//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "shapes.c"

//add GPIO pointer, timer pointer, VGA pointer

//s

int score;
char grid[][];

void render(){

}

void collision_detect(){

}

void spawn_shape(int rand){

}

void interrupt_handler(unsigned int cause){
    switch (cause){
        case 18:
            break;
    }
}

void loop(){
    
}