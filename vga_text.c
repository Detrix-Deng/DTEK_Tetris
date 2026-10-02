#include "dtekv-lib.h"

#define WIDTH 320
#define HEIGHT 240

void console_out(char array[HEIGHT][WIDTH]){ //dump contents of buffer into console
    for(int col = 0; col < HEIGHT; col++){
        for(int row = 0; row < WIDTH; row++){
            if(array[col][row] == 255){
                print("1");
            } else {
                print("0");
            }
        }
        print("\n");
    }
}