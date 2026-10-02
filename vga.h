#ifndef VGA_H
#define VGA_H

#include <stdio.h>
#include "config.h"

void put_grid(char array[HEIGHT][WIDTH], char grid[GRID_HEIGHT][GRID_WIDTH], int x_offset, int y_offset, int id);
//Draw sprite onto VGA buffer
void put_sprite(char array[HEIGHT][WIDTH], char sprite[4][4], int x_offset, int y_offset, bool is_curr, int id);
//Feed in matrix, text, offset positions
void put_text(char array[HEIGHT][WIDTH], char *text, int x_offset, int y_offset, int scalar);

void render(char array[HEIGHT][WIDTH]);

void clear_display(char array[HEIGHT][WIDTH]);

#endif // "VGA_H"