#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h> // for rand

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
    },
    {6, {//Test shape
            {1,1,1,1},
            {1,1,0,0},
            {0,1,1,0},
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

void print_grid(char grid[GRID_HEIGHT][GRID_WIDTH]){
    printf("   ");
    for(int i = 0; i < GRID_WIDTH; i++)
        printf("%2d", i);
    printf("\n");
    for(int i = 0; i < GRID_HEIGHT; i++){
        printf("%2d ", i);
        for(int j = 0; j < GRID_WIDTH; j++){
            printf("%2d", (int) grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void place_sprite(int player){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            player_list[player].grid[player_list[player].curr_sprite.y - j][player_list[player].curr_sprite.x + i] |= player_list[player].curr_sprite.sprite_shape.matrix[3 - j][i];
        }
    }
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

void rotate(int player, bool inspect){
    // rotate player's sprite clockwise
    char new_matrix[4][4];
    // Rotate and save in new_matrix
    for(int i = 0; i < 4; i++){
        for(int j = 3; j >= 0; j--){
            new_matrix[i][3-j] = player_list[player].curr_sprite.sprite_shape.matrix[j][i];
        }
    }
    if(inspect){
        printf("Sprite after rotation:\n");
        print_sprite(new_matrix);
    }
    // Check minimum distance to left border
    int min_dist = 3;
    for(int i = 0; i < 4; i++){
        int dist = 0;
        for(int j = 0; j < 4; j++){
            if(new_matrix[i][j]){
                break;
            }
            dist++;
        }
        if(dist < min_dist)
            min_dist = dist;
    }
    if(inspect){
        printf("Min dist = %d\n", min_dist);
    }
    // Shift and transfer new_matrix to player sprite
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4 - min_dist; j++){
            player_list[player].curr_sprite.sprite_shape.matrix[i][j] = new_matrix[i][j + min_dist];
        }
        for(int j = 4 - min_dist; j < 4; j++){
            player_list[player].curr_sprite.sprite_shape.matrix[i][j] = 0;
        }
    }
    if(inspect){
        printf("Sprite after shift:\n");
        print_sprite(player_list[player].curr_sprite.sprite_shape.matrix);
    }
    border_detect(player);
}

void spawn_sprite(int player, int index){     // Update curr_sprite with next_sprite
    int rand_int = rand() % 6;
    player_list[player].next_sprite[index].sprite_shape = sprite_shapes[rand_int];
    player_list[player].next_sprite[index].x = (GRID_WIDTH / 2) - 2;   // CHANGE!! (player * field) + offset;
    player_list[player].next_sprite[index].y = 4 - 1; // CHANGE!! offset;
}

void get_next_sprite(int player){               // Update curr_sprite with next_sprite
    player_list[player].curr_sprite = player_list[player].next_sprite[0];
    player_list[player].next_sprite[0] = player_list[player].next_sprite[1];
    player_list[player].next_sprite[1] = player_list[player].next_sprite[2];
    spawn_sprite(player, 2);
    player_list[player].hold_available = true;
}

void player_init(bool multiplayer){
    for(int i = 0; i <= multiplayer; i++){
        player_list[i].score = 0;
        player_list[i].lines = 0;
        player_list[i].to_count = 0;
        player_list[i].difficulty = 0;
        player_list[i].mytime = 0x0000;
        // time2string(player_list[i].textstring, player_list[i].mytime);

        for(int j = 0; j < 3; j++)
            spawn_sprite(i, j);
        get_next_sprite(i);
        // player_list[i].curr_sprite.sprite_shape = sprite_shapes[test_id];
        for(int row = 0; row < GRID_HEIGHT; row++){
            for(int col = 0; col < GRID_WIDTH; col++){
                player_list[i].grid[row][col] = 0;
            }
        }

        player_list[i].hold = -1;
        // hold_available is automatically true from get_next_sprite

        player_list[i].lost = false;
    }
}

void get_player_info(int player){
    printf("Player score: %d\n", player_list[player].score);
    printf("Player lines: %d\n", player_list[player].lines);
    printf("Player to_count: %d\n", player_list[player].to_count);
    printf("Player difficulty: %d\n", player_list[player].difficulty);
    printf("Player mytime: %d\n", player_list[player].mytime);
    printf("Player hold: %d\n", player_list[player].hold);
    printf("Player sprite_id: %d\n", player_list[0].curr_sprite.sprite_shape.sprite_id);
    printf("Player sprite:\n");
    print_sprite(player_list[player].curr_sprite.sprite_shape.matrix);
    printf("Player x: %d\n", player_list[player].curr_sprite.x);
    printf("Player y: %d\n", player_list[player].curr_sprite.y);
    printf("Player lost: ");
    if(player_list[player].lost)
        printf("True\n");
    else
        printf("False\n");
    print_grid(player_list[0].grid);
}

bool check_sprite(char matrix[4][4], int sprite_id){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(matrix[i][j] == sprite_shapes[sprite_id].matrix[i][j]){
                continue;
            }
            else{
                return false;
            }
        }
    }
    return true;
}

void test_sprite(int test_id, bool inspect){
    player_list[0].curr_sprite.sprite_shape = sprite_shapes[test_id];
    if(inspect){
        printf("Test sprite:\n");
        print_sprite(player_list[0].curr_sprite.sprite_shape.matrix);
    }
    rotate(0, inspect);
    rotate(0, inspect);
    rotate(0, inspect);
    rotate(0, inspect);
    printf("Test sprite %d: ", test_id);
    if(check_sprite(player_list[0].curr_sprite.sprite_shape.matrix, test_id))
        printf("True\n");
    else
        printf("False\n");
}

void main(){
    player_init(false);
    get_player_info(0);
    // for(int i = 0; i <= 6; i++)
    //     test_sprite(i, false);
}