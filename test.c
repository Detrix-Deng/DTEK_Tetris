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

extern void time2string(char*,int);

void print_sprite(char matrix[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%d", (int) matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void border_detect(int player){
    int oob = 0;    //oob = out_of_bounds
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++){
            if(player_list[player].curr_sprite.sprite_shape.matrix[i][j]){
                if((player_list[player].curr_sprite.x + j - oob) >= GRID_WIDTH)
                    oob += 8 - player_list[player].curr_sprite.x + j - oob;
            }
        }
    }
    player_list[player].curr_sprite.x = player_list[player].curr_sprite.x - oob;
}

void rotate(int player){
    // rotate curr_sprite[player] right or left based on direction
    unsigned char new_shape[4] = {0};
    char test_matrix[4][4];
    // Rotate and save shape as an array of bits
    for(int i = 0; i < 4; i++){
        for(int j = 3; j >= 0; j--){
            new_shape[i] = new_shape[i] | ((player_list[player].curr_sprite.sprite_shape.matrix[j][i]) << (3 - j));
            test_matrix[i][3-j] = player_list[player].curr_sprite.sprite_shape.matrix[j][i];
        }
    }
    print_sprite(test_matrix);
    // Shift left
    int min_dist = 3;
    for(int i = 0; i < 4; i++){
        int dist = 0;
        // if(new_shape[i] == 0)
        //     continue;
        // for(int j = 3; j >= 0; j--){
        //     if((new_shape[i] >> j) & 0x01)
        //         break;
        //     dist++;
        // }
        // if(dist < min_dist)
        //     min_dist = dist;
        for(int j = 0; j < 4; j++){
            if(test_matrix[i][j]){
                break;
            }
            dist++;
        }
        if(dist < min_dist)
            min_dist = dist;
    }
    printf("%d\n", min_dist);
    // Translate and save new_shape to curr_sprite
    // for(int i = 0; i < 4; i++)
    //     new_shape[i] = new_shape[i] << min_dist;
    // for(int i = 0; i < 4; i++){
    //     for(int j = 3; j >= 0; j--)
    //         player_list[player].curr_sprite.sprite_shape.matrix[i][3 - j] = (new_shape[i] >> j) & 0x01;
    // }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < min_dist; j++){
            player_list[player].curr_sprite.sprite_shape.matrix[i][j] = test_matrix[i][j + min_dist];
        }
        for(int j = min_dist; j < 4; j++){
            player_list[player].curr_sprite.sprite_shape.matrix[i][j] = 0;
        }
    }
    print_sprite(player_list[player].curr_sprite.sprite_shape.matrix);
    border_detect(player);
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
    // for(int i = 0; i < 6; i++)
    //     print_sprite(sprite_shapes[i].matrix);
    player_list[0].curr_sprite.sprite_shape = sprite_shapes[1];
    print_sprite(player_list[0].curr_sprite.sprite_shape.matrix);
    rotate(0);
    print_sprite(player_list[0].curr_sprite.sprite_shape.matrix);
}