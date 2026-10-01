//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include "shapes.c"
#include "vga.c"
// #include "main.c"

//add GPIO pointer, timer pointer, VGA pointer
volatile int *gpio = (volatile int *) 0x040000e0;

// 

int OFFSET_X1; //Offset for player 1
int OFFSET_X2; //Offset for player 2
int OFFSET_Y = 109;
#define GRID_WIDTH 10
#define GRID_HEIGHT 20
char VGA[HEIGHT][WIDTH];  //vga buffer

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

struct player_info player_list[2];
int score[2] = {0};
char hold[2] = {-1, -1};      // Stores the type_id of the hold sprite
char grid[2][GRID_HEIGHT][GRID_WIDTH] = {0};    //OBS REVERSE WIDTH AND HEIGHT WHEN COPYING TO VGA PREBUFFER
struct sprite curr_sprite[2];   // array with 2 struct of curr_sprite for each player
struct sprite next_sprite[2][3];  // 2 lists containing the 3 upcoming sprite.
int to_count = 0;   // Counter for TO flags
int difficulty = 0;    // Determines how fast mov_down is called (level)
bool hold_available[2]; // Bool for if hold action can be used
bool multiplayer = false;   // Bool for whether session is 1P or 2P
bool lost[2] = {false};     // Bool for if player has lost
int lines[2] = {0};     // Number of lines each player has cleared
int mytime[2] = {0x0000, 0x0000};    // Player specific timer
char textstring[2][6] = {"00:00", "00:00"};

void set_offset(bool multiplayer){
    // Sets pixel offset of the playing field depending on multiplayer
    if(multiplayer){
        OFFSET_X1 = 74;
        OFFSET_X2 = 234;
    } 
    else
        OFFSET_X1 = 154;
        OFFSET_X2 = 0; //zero offset two so it isnt junk value
}

void place_sprite(int player){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            grid[player][curr_sprite[player].y - j][curr_sprite[player].x + i] |= curr_sprite[player].sprite_shape.matrix[3 - j][i];
        }
    }
}

unsigned int get_rand(){
    // take the snapL of timer, and then do some calculation to generate a random int.
    volatile int *time_addr = (volatile int *)0x04000020;
    time_addr += 4; //snapL
    unsigned int rand = *time_addr;
    return rand;
}

bool collision_detect(int player){    // Check if the space below the sprite is occupied.
    // If occupied, return True, else, return False
    bool collision = false;
    for(int i = 0; i < 4; i++){
        int height_offset = curr_sprite[player].y;
        int j = 3;
        while(j >= 0 && !(curr_sprite[player].sprite_shape.matrix[j][i]))
            j--;
        height_offset = height_offset - j;
        if(j < 0)
            break;
        else if(grid[player][height_offset + 1][curr_sprite[player].x])
            collision = true;
    }
    return collision;
}

void border_detect(int player){
    int oob = 0;    //oob = out_of_bounds
    for(int i = 0; i < 2; i++){
        for(int j = 0; j < 4; j++){
            if(curr_sprite[player].sprite_shape.matrix[i][j]){
                if((curr_sprite[player].x + j - oob) >= GRID_WIDTH)
                    oob += 8 - curr_sprite[player].x + j - oob;
            }
        }
    }
    curr_sprite[player].x = curr_sprite[player].x - oob;
}

void spawn_sprite(int player, int index){     // Update curr_sprite with next_sprite
    int rand_int = get_rand() % 6;
    next_sprite[player][index].sprite_shape = sprite_shapes[rand_int];
    next_sprite[player][index].x = (GRID_WIDTH / 2) - 1;   // CHANGE!! (player * field) + offset;
    next_sprite[player][index].y = 4 - 1; // CHANGE!! offset;
}

void get_next_sprite(int player){
    curr_sprite[player] = next_sprite[player][0];
    next_sprite[player][0] = next_sprite[player][1];
    next_sprite[player][1] = next_sprite[player][2];
    spawn_sprite(player, 2);
    hold_available[player] = true;
}

void score_calc(int players, int line){  // Calculates/update game score
    int mult = line * 1000;
    score[players] += mult; //add switch cases later
}

int line_clear(int player){   // After collision detect == True
    // Check if any relevant layer is full.
    // If layer is full, layer_cleared++
    // move everything in grid above the lowest cleared
    // layer by layer_cleared amount
    int layer_cleared = 0;
    char clear_level[GRID_HEIGHT] = {0};
    // Check relevant layer
    for(int i = curr_sprite[player].y; i < (curr_sprite[player].y + 4); i--){
        int j = 0;
        while((curr_sprite[player].sprite_shape.matrix[i][j]) && j < GRID_WIDTH)
            j++;
        if(j == GRID_WIDTH){
            layer_cleared++;
            clear_level[i] = 1;
        }
    }
    // Clear and move layer down
    int distance = 0;
    if(layer_cleared){
        for(int i = GRID_HEIGHT - 1; i >= layer_cleared; i--){
            if(clear_level[i])
                distance++;
            if(distance){
                for(int j = 0; j < GRID_WIDTH; j++){
                    grid[player][i][j] = grid[player][i + distance][j];
                }
            }
        }
        for(int i = 0; i < layer_cleared; i++){
            for(int j = 0; j < GRID_WIDTH; j++){
                grid[player][GRID_HEIGHT - i - 1][j] = 0;
            }
        }
    }
    return layer_cleared;
}

void rotate(int player){
    // rotate curr_sprite[player] right or left based on direction
    unsigned char new_shape[4] = {0};
    // Rotate and save shape as an array of bits
    for(int i = 0; i < 4; i++){
        for(int j = 3; j >= 0; j--)
            new_shape[i] = new_shape[i] | ((curr_sprite[player].sprite_shape.matrix[j][i]) << (3 - j));
    }
    // Shift left
    int min_dist = 3;
    for(int i = 0; i < 4; i++){
        int dist = 0;
        if(new_shape[i] == 0)
            continue;
        for(int j = 3; j >= 0; j--){
            if((new_shape[i] >> j) & 0x01)
                break;
            dist++;
        }
        if(dist < min_dist)
            min_dist = dist;
    }
    // Translate and save new_shape to curr_sprite
    for(int i = 0; i < 4; i++)
        new_shape[i] = new_shape[i] << min_dist;
    for(int i = 0; i < 4; i++){
        for(int j = 3; j >= 0; j--)
            curr_sprite[player].sprite_shape.matrix[i][3 - j] = (new_shape[i] >> j) & 0x01;
    }
    border_detect(player);
}

void mov_down(int player){    // y in curr_shape += 1
    // This is polled every game cycle
    // Call line_clear if collision_check
    // If line_clear > 0, call score_calc
    if(collision_detect(player)){
        place_sprite(player);
        int lines = line_clear(player);
        if(lines){
            score_calc(player, lines);
        }
        get_next_sprite(player);
        spawn_sprite(player, 2);
    }
    else{
        curr_sprite[player].y++;
    }
}

void hard_down(int player){
    // move sprite all the way down
    do{
        move_down(player);  // Risk for double terimino hard down if interrupt exactly when
                            // do-while loop is done
    } while(curr_sprite[player].y > 3);
}

void mov_hor(int player, int direction){   // x in curr_shape +- 1, depending on direction
    curr_sprite[player].x += direction;
    if(curr_sprite[player].x < 0)
        curr_sprite[player].x == 0;
    else if(curr_sprite[player].x >= GRID_WIDTH)
        border_detect(player);
}

void hold_func(int player){
    if(hold[player] < 0){
        hold[player] = curr_sprite[player].sprite_shape.sprite_id;
        get_next_sprite(player);
    }
    else{
        int temp = hold[player];
        hold[player] = curr_sprite[player].sprite_shape.sprite_id;
        curr_sprite[player].sprite_shape = sprite_shapes[temp];
        curr_sprite[player].y = 4 - 1;
        curr_sprite[player].x = (GRID_WIDTH / 2) - 1;
    }
}

void interrupt_handler(unsigned int cause){
    volatile int *time_addr = (volatile int *)0x04000020;
    *time_addr = 2; // Clear TO flag
    render(VGA);
    to_count++;
    if(to_count >= 45 - difficulty){
        to_count = 0;
        if(multiplayer)
            mov_down(2);
        mov_down(1);
    }
}

void player_init(bool multiplayer){
    for(int i = 0; i <= multiplayer; i++){
        player_list[i].score = 0;
        player_list[i].lines = 0;
        player_list[i].to_count = 0;
        player_list[i].difficulty = 0;
        player_list[i].mytime = 0x0000;
        time2string(player_list[i].textstring, mytime);

        for(int j = 0; j < 3; j++)
            spawn_sprite(i, j);
        get_next_sprite(i);

        player_list[i].hold = -1;
        // hold_available is automatically true from get_next_sprite

        player_list[i].lost = false;
    }
}

void loop(){    // game loop
    // poll inputs
    int rot1 = *gpio & 0x0001;
    int mv_r1 = *gpio & 0x0002;
    int mv_l1 = *gpio & 0x0004;
    int down1 = *gpio & 0x0008;
    int hold1 = *gpio & 0x0010;
    int rot2 = *gpio & 0x0020;
    int mv_r2 = *gpio & 0x0040;
    int mv_l2 = *gpio & 0x0080;
    int down2 = *gpio & 0x0100;
    int hold2 = *gpio & 0x0200;

    // Player 1
    if (rot1){
        rotate(1);
    }
    if(mv_r1){
        mov_hor(1, 1);
    }
    if(mv_l1){
        mov_hor(1, -1);
    }
    if(down1){
        hard_down(1);
    }
    if(hold1 && hold_available[0]){
        hold_available[0] = false;
        hold_func(1);
    }

    // Player 2
    if(multiplayer)
    {
        if (rot2){
            rotate(2);
        }
        if(mv_r2){
            mov_hor(2, 1);
        }
        if(mv_l2){
            mov_hor(2, -1);
        }
        if(down2){
            hard_down(2);
        }
        if(hold2 && hold_available[1]){
            hold_available[1] = false;
            hold_func(2);
        }
    }
}