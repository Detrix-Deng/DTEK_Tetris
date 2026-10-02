//main stuff

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "tetris.c"
#include "vga.c"
#include "shapes.c"

void main(){
    //setup board things, enable interrupt, etc
    put_text(VGA, "TETRIS", 0, 0, 5);
    put_sprite(VGA, sprite_shapes[0], 50, 50, false, 0);
    put_grid(VGA, player_list[0].grid, 100, 100, 0);
    render(VGA);
}