#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include "config.h"

// Hardware
extern volatile int *gpio;

// Offset
extern int OFFSET_X1;
extern int OFFSET_X2;
extern int OFFSET_Y;

// Struct

struct sprite_shape {
    char sprite_id;
    char matrix[4][4];
};

struct sprite{
    struct sprite_shape sprite_shape;
    int x;
    int y;
};

struct player_info{
    // UI element
    int score;                              // Score
    int lines;                              // Lines cleared
    int to_count;                           // TO passed since last game cycle
    int difficulty;                         // How fast a game cycle is (Level)
    int mytime;                             // Total time passed
    char textstring[6];                     // Time as a string, ex. 00:16

    // Sprite and grid info
    struct sprite curr_sprite;              // Info of current sprite
    struct sprite next_sprite[3];           // Info of next sprite
    char grid[GRID_HEIGHT][GRID_WIDTH];     // Info of player's grid

    // Hold info
    char hold;                              // Info of hold slot
    bool hold_available;                    // Bool whether hold action is available

    bool lost;                              // Bool for if player has lost
};

// Global variable
extern struct sprite_shape sprite_shapes[];
extern struct player_info player_list[2];
extern unsigned int global_to_count;
extern bool multiplayer;

// Functions

void set_offset(bool multiplayer);

void place_sprite(int player);

unsigned int get_rand(void);

bool collision_detect(int player);

void border_detect(int player);

void spawn_sprite(int player, int index);

void get_next_sprite(int player);

void score_calc(int player, int line);

int line_clear(int player);

void rotate(int player);

void mov_down(int player);

void hard_down(int player);

void mov_hor(int player, int direction);

void hold_func(int player);

void interrupt_handler(unsigned int cause);

void player_init(bool multiplayer);

void loop(void);

#endif