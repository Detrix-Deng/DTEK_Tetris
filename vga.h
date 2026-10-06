#ifndef VGA_H
#define VGA_H

#include <stdio.h>
#include "config.h"

extern void put_grid(char array[WIDTH][HEIGHT], char grid[GRID_HEIGHT][GRID_WIDTH], int x_offset, int y_offset, int scalar);
//Draw sprite onto VGA buffer
extern void put_sprite(char array[WIDTH][HEIGHT], char sprite[4][4], int x_offset, int y_offset, bool is_curr, int id, int scalar);
//Feed in matrix, text, offset positions
extern void put_text(char array[WIDTH][HEIGHT], char *text, int x_offset, int y_offset, int scalar);

extern void put_line(char array[WIDTH][HEIGHT], int x, int y, int length, int width);

extern void render(char array[WIDTH][HEIGHT]);

extern void clear_display(char array[WIDTH][HEIGHT]);

#endif // "VGA_H"