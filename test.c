#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

struct sprite_shape {
    char sprite_id;
    char matrix[4][4];
};

struct sprite_shape sprite_shapes[] = {
    {0, {//L
            {1,0,0,0},
            {1,0,0,0},
            {1,1,0,0},
            {0,0,0,0}
        }
    },
    {1, {//T
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    {2, {//Cube
            {1,1,0,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    {3, {//Z
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    {4, {//I
            {1,0,0,0},
            {1,0,0,0},
            {1,0,0,0},
            {1,0,0,0}
        }
    },
    {5, {//S
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    }
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

struct player_info player_list[1];

void time2string(char*,int);

void print_sprite(char matrix[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// void player_init(bool multiplayer){
//     for(int i = 0; i <= multiplayer; i++){
//         player_list[i].score = 0;
//         player_list[i].lines = 0;
//         player_list[i].to_count = 0;
//         player_list[i].difficulty = 0;
//         player_list[i].mytime = 0x0000;
//         time2string(player_list[i].textstring, player_list[i].mytime);

//         for(int j = 0; j < 3; j++)
//             spawn_sprite(i, j);
//         get_next_sprite(i);

//         player_list[i].hold = -1;
//         // hold_available is automatically true from get_next_sprite

//         player_list[i].lost = false;
//     }
// }

void main(){
    print_sprite(sprite_shapes[0].matrix);
}