//for tetris related, maybe VGA?
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dtekv-lib.h"
#include "tetris.h"
#include "shapes.h"
#include "vga.h"
#include "config.h"

//add GPIO pointer, timer pointer, VGA pointer
// volatile int *gpio = (volatile int *) 0x040000e0;
// volatile int *direction = (volatile int *) 0x040000e4;

int OFFSET_X[2]; //Offset for player 1
int OFFSET_Y = 89;
char VGA[HEIGHT][WIDTH];  //vga buffer

// functions from timetemplate from lab 3
extern void time2string(char*,int);
extern void tick(int*);

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

struct player_info player_list[2];
unsigned int global_to_count = 0; //to means timeout
bool multiplayer = false;   // Bool for whether session is 1P or 2P

void set_offset(bool multiplayer){ //Contributed by both
    // Sets pixel offset of the playing field depending on multiplayer
    if(multiplayer){
        OFFSET_X[0] = 64;
        OFFSET_X[1] = 224;
    } 
    else{
        OFFSET_X[0] = 144;
        OFFSET_X[1] = 0; //zero offset so it isnt junk value
    }
}
// Contributed by Dave
int read_gpio(){
    // Function used to reset and read gpio
    // Change direction to be able to write to gpio
    *direction = 0xFFFFFFFF;
    // Reset gpio due to it always sets itself to 0xFFFFFFFF
    *outclear = 0xFFFFFFFF;
    // Change direction to be able to write to gpio externally
    *direction = 0x00;
    // asm nop because otherwise reading too fast
    asm volatile ("nop");
    return *gpio;
}

//Contributed by Dave
void place_sprite(int player){      // Test passed
    int layers_exclude = player_list[0].curr_sprite.y;
    if((GRID_HEIGHT - layers_exclude) <= 0)
        layers_exclude = layers_exclude - GRID_HEIGHT;
    else
        layers_exclude = 0;
    for(int i = 0; i < 4 - layers_exclude; i++){
        for(int j = 0; j < 4; j++){
            player_list[player].grid[player_list[player].curr_sprite.y - j][player_list[player].curr_sprite.x + i] |= player_list[player].curr_sprite.sprite_shape.matrix[3 - j][i];
        }
    }
    player_list[player].hold_available = true;
    // Update player's grid in buffer
    put_grid(VGA, player_list[player].grid, OFFSET_X[player] + 3 * player_list[player].curr_sprite.x, 
             OFFSET_Y - 3 + 3 * player_list[player].curr_sprite.y, 3);
}

//Contributed by Dave
unsigned int get_rand(){
    // take the snapL of timer, and then do some calculation to generate a random int.
    volatile int *time_addr = (volatile int *)0x04000020;
    time_addr += 4; //snapL
    unsigned int rand = *time_addr;
    return rand;
}

//Contributed by Dave
bool collision_detect(int player){    // Test passed
    // Check if the space below the sprite is occupied.
    // If occupied, return True, else, return False
    bool collision = false;
    for(int i = 0; i < 4; i++){
        int height_offset = player_list[player].curr_sprite.y;
        int j = 3;
        while(j >= 0 && !(player_list[player].curr_sprite.sprite_shape.matrix[j][i]))
            j--;
        height_offset = height_offset - 3 + j;
        if(j < 0)
            break;
        else if(height_offset + 1 == GRID_HEIGHT || player_list[player].grid[height_offset + 1][player_list[player].curr_sprite.x + i]){
            collision = true;
            if(player_list[player].curr_sprite.y == 3){
                player_list[player].lost = true;
            }
        }
    }
    return collision;
}

//Contributed by Dave
void border_detect(int player){     // Test passed
    int oob = 0;    //oob = out_of_bounds
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 4; j++){
            if(player_list[player].curr_sprite.sprite_shape.matrix[i][j]){
                if((player_list[player].curr_sprite.x + j - oob) >= GRID_WIDTH)
                    oob = (player_list[player].curr_sprite.x + j) - (GRID_WIDTH - 1);
            }
        }
    }
    player_list[player].curr_sprite.x = player_list[player].curr_sprite.x - oob;
}

//Contributed by Dave
void spawn_sprite(int player, int index){     // Test passed
    // Update curr_sprite with next_sprite
    int rand_int = get_rand() % 6;
    player_list[player].next_sprite[index].sprite_shape = sprite_shapes[rand_int];
    player_list[player].next_sprite[index].x = (GRID_WIDTH / 2) - 1;
    player_list[player].next_sprite[index].y = 4 - 1;
    // Update player's next sprite in buffer
    put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] + 3 * GRID_WIDTH + 3 + 1,
               OFFSET_Y - (2 * 4 + 1) * index, false, player, 2);
}

//Contributed by Dave
void get_next_sprite(int player){       // Test passed
    // Update curr_sprite with next_sprite
    player_list[player].curr_sprite = player_list[player].next_sprite[0];
    player_list[player].next_sprite[0] = player_list[player].next_sprite[1];
    player_list[player].next_sprite[1] = player_list[player].next_sprite[2];
    spawn_sprite(player, 2);
    // Update player's curr sprite in buffer
    put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] + 3 * player_list[player].curr_sprite.x,
               OFFSET_Y - 3 * 3, true, player, 3);
}

//Contributed by Dave
void score_calc(int player, int line){      // Test passed
    // Calculates/update game score based on number of lines cleared and difficulty level
    switch(line){
        case 1:
            player_list[player].score += 100 * player_list[player].difficulty;
            break;
        
        case 2:
            player_list[player].score += 300 * player_list[player].difficulty;
            break;

        case 3:
            player_list[player].score += 500 * player_list[player].difficulty;
            break;

        case 4:
            player_list[player].score += 800 * player_list[player].difficulty;
            break;

        default:
            break;
    }
    // Update player's score in buffer
    put_text(VGA, (char*) player_list[player].score, OFFSET_X[player] + 36, OFFSET_Y - 35, 1);
}

//Contributed by Dave
int line_clear(int player){   // Test passed
    // After collision detect == True
    // Check if any relevant layer is full.
    // If layer is full, layer_cleared++
    // move everything in grid above the lowest cleared
    // layer by layer_cleared amount
    int layer_cleared = 0;
    char level_empty[GRID_HEIGHT] = {0};
    // Check relevant layer
    for(int i = player_list[player].curr_sprite.y; i > (player_list[player].curr_sprite.y - 4); i--){
        int j = 0;
        while((player_list[player].grid[i][j]) && j < GRID_WIDTH)
            j++;
        if(j == GRID_WIDTH){
            layer_cleared++;
            level_empty[i] = 1;
        }
    }
    // Clear and move layer down
    int temp_y = GRID_HEIGHT - 1;
    if(layer_cleared){
        for(int i = GRID_HEIGHT - 2; i >= 0; i--){
            temp_y = i + 1;
            if(level_empty[i]){
                continue;
            }
            else{
                while((temp_y < GRID_HEIGHT - 1) && level_empty[temp_y]){
                    temp_y++;
                }
                if(!level_empty[temp_y])
                    temp_y--;
                for(int j = 0; j < GRID_WIDTH; j++){
                    player_list[player].grid[temp_y][j] = player_list[player].grid[i][j];
                }
                level_empty[temp_y] = 0;
                level_empty[i] = 1;
            }
        }
        // From layer 0 to layer layer_cleared, fill with 0;
        for(int i = 0; i < layer_cleared; i++){
            for(int j = 0; j < GRID_WIDTH; j++){
                player_list[player].grid[i][j] = 0;
            }
        }
    }
    player_list[player].lines += layer_cleared;
    return layer_cleared;
}

//Contributed by Dave
void rotate(int player){    // Test passed
    // rotate player's sprite clockwise
    char new_matrix[4][4];
    // Rotate and save in new_matrix
    for(int i = 0; i < 4; i++){
        for(int j = 3; j >= 0; j--){
            new_matrix[i][3-j] = player_list[player].curr_sprite.sprite_shape.matrix[j][i];
        }
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
    // Shift and transfer new_matrix to player sprite
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4 - min_dist; j++){
            player_list[player].curr_sprite.sprite_shape.matrix[i][j] = new_matrix[i][j + min_dist];
        }
        for(int j = 4 - min_dist; j < 4; j++){
            player_list[player].curr_sprite.sprite_shape.matrix[i][j] = 0;
        }
    }
    border_detect(player);
    // Update player's curr sprite in buffer
    put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] + 3 * player_list[player].curr_sprite.x, 
               OFFSET_Y - 3 + 3 * (player_list[player].curr_sprite.y), true, player, 3);
}

//Contributed by Dave
void mov_down(int player){    // Test passed
    // y in curr_shape += 1
    // This is polled every game cycle
    // Call line_clear if collision_check
    // If line_clear > 0, call score_calc
    if(collision_detect(player)){
        place_sprite(player);
        player_list[player].score += 1;
        int lines = line_clear(player);
        if(lines){
            // Update player's lines in buffer
            put_text(VGA, (char*) player_list[player].lines, OFFSET_X[player] + 36, OFFSET_Y - 27, 1);
            score_calc(player, lines);
        }
        get_next_sprite(player);
        spawn_sprite(player, 2);
        // Update player's score in buffer
        put_text(VGA, (char*) player_list[player].score, OFFSET_X[player] + 36, OFFSET_Y - 35, 1);
    }
    else{
        player_list[player].curr_sprite.y++;
        // Update player's curr sprite in buffer
        put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] + 3 * player_list[player].curr_sprite.x, 
               OFFSET_Y - 3 + 3 * (player_list[player].curr_sprite.y), true, player, 3);
        print("Moved down!");
    }
}

//Contributed by Dave
void hard_down(int player){     // Test passed
    // move sprite all the way down
    do{
        mov_down(player);  // Risk for double terimino hard down if interrupt exactly when
                            // do-while loop is done
    } while(player_list[player].curr_sprite.y > 3);
    player_list[player].score += 1;
    // Update player's score in buffer
    put_text(VGA, (char*) player_list[player].score, OFFSET_X[player] + 36, OFFSET_Y - 35, 1);
}

//Contributed by Dave
void mov_hor(int player, int direction){   // Test passed
    // x in curr_shape +- direction, where directions is a parameter
    player_list[player].curr_sprite.x += direction;
    if(player_list[player].curr_sprite.x < 0)
        player_list[player].curr_sprite.x = 0;
    else if((player_list[player].curr_sprite.x) >= (GRID_WIDTH - 4))
        border_detect(player);
    put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] + 3 * player_list[player].curr_sprite.x, 
               OFFSET_Y + 3 * (player_list[player].curr_sprite.y - 3), true, player, 3);
}

//Contributed by Dave
void hold_func(int player){     // Test passed
    if(player_list[player].hold_available){
        player_list[0].hold_available = false;
        if(player_list[player].hold < 0){
            player_list[player].hold = player_list[player].curr_sprite.sprite_shape.sprite_id;
            get_next_sprite(player);
        }
        else{
            int temp = player_list[player].hold;
            player_list[player].hold = player_list[player].curr_sprite.sprite_shape.sprite_id;
            player_list[player].curr_sprite.sprite_shape = sprite_shapes[temp];
            player_list[player].curr_sprite.y = 4 - 1;
            player_list[player].curr_sprite.x = (GRID_WIDTH / 2) - 1;
            // Update player's curr sprite in buffer
            put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] + 3 * player_list[player].curr_sprite.x,
                   OFFSET_Y - 3 + 3 * player_list[player].curr_sprite.y, true, player, 3);
        }
        // Update player's hold sprite in buffer
        put_sprite(VGA, player_list[player].curr_sprite.sprite_shape.matrix, OFFSET_X[player] - 3 - 1 - 4 * 2,
                   OFFSET_Y, false, player, 2);
    }
}

//Contributed by Dave
void increase_difficulty(int player){   
    // Increases the player's difficulty when called up to a maximum of 15
    if(player_list[player].difficulty < 15){
        player_list[player].difficulty++;
    }
    // Update player's level value in buffer
    put_text(VGA, (char*) player_list[player].difficulty, OFFSET_X[player] + 36, OFFSET_Y - 19, 1);
}

//Contributed by Both
void handle_interrupt(unsigned int cause){
    volatile int *time_addr = (volatile int *)0x04000020;
    *time_addr = 2; // Clear TO flag
    // render(VGA);
    
    global_to_count++;
    if(global_to_count >= 30){
        render(VGA);
        global_to_count = 0;
        for(int player = 0; player <= multiplayer; player++){
            if(!player_list[player].lost){
                // Increase the player's time by 1 if not lost
                tick(&player_list[player].mytime);
                time2string(player_list[player].textstring, player_list[player].mytime);
                // Update player's time in buffer
                put_text(VGA, player_list[player].textstring, OFFSET_X[player], OFFSET_Y - 11, 1);
            }
        }
        for(int player = 0; player <= multiplayer; player++){
            if(((player_list[player].mytime >> 4) * 10 + player_list[player].mytime) % 20 == 0 && !player_list[player].lost){
                // increase difficulty for every 20 sec
                increase_difficulty(player);
            }
        }
    }

    for(int player = 0; player <= multiplayer; player++){
        player_list[player].to_count++;
        if(player_list[player].to_count >= 45 - 3 * (player_list[player].difficulty - 1)){
            // 45 = 1.5 seconds
            // -3*difficulty = 0.1 seconds faster for every difficulty level
            // max difficulty 15 = mov_down once every 0.1 seconds
            player_list[player].to_count = 0;
            mov_down(player);
        }
    }
}

//Contributed by Dave
void player_init(bool multiplayer){
    for(int player = 0; player <= multiplayer; player++){
        player_list[player].score = 0;
        player_list[player].lines = 0;
        player_list[player].to_count = 0;
        player_list[player].difficulty = 1;
        player_list[player].mytime = 0x0000;
        time2string(player_list[player].textstring, player_list[player].mytime);

        for(int j = 0; j < 3; j++)
            spawn_sprite(player, j);
        get_next_sprite(player);
        for(int row = 0; row < GRID_HEIGHT; row++){
            for(int col = 0; col < GRID_WIDTH; col++){
                player_list[player].grid[row][col] = 0;
            }
        }

        player_list[player].hold = -1;
        player_list[player].hold_available = true;

        player_list[player].lost = false;

        // Text on display
        put_text(VGA, "SCORE:", OFFSET_X[player], OFFSET_Y - 35, 1);
        put_text(VGA, "LINES:", OFFSET_X[player], OFFSET_Y - 27, 1);
        put_text(VGA, "LEVEL:", OFFSET_X[player], OFFSET_Y - 19, 1);
        put_text(VGA, player_list[player].textstring, OFFSET_X[player], OFFSET_Y - 11, 1);
        put_text(VGA, (char*) player_list[player].score, OFFSET_X[player] + 36, OFFSET_Y - 35, 1);
        put_text(VGA, (char*) player_list[player].lines, OFFSET_X[player] + 36, OFFSET_Y - 27, 1);
        put_text(VGA, (char*) player_list[player].difficulty, OFFSET_X[player] + 36, OFFSET_Y - 19, 1);
        put_grid(VGA, player_list[player].grid, OFFSET_X[player], OFFSET_Y, 3);

        // Draw Hold and Next Grid manually
        // Hold grid
        put_line(VGA, OFFSET_X[player] - 3 - (2 * 4) - 2, OFFSET_Y - 3, (2 * 4) + 2, 2);
        put_line(VGA, OFFSET_X[player] - 3 - (2 * 4) - (2 * 2), OFFSET_Y - 3, 2, (2 * 4) + (2 * 2) + 2);
        put_line(VGA, OFFSET_X[player] - 3 - (2 * 4) - 2, OFFSET_Y + (2 * 4) + 1, (2 * 4) + 2, 2);

        // Next grid
        put_line(VGA, OFFSET_X[player] + (3 * GRID_WIDTH) + 3, OFFSET_Y - 3, (2 * 4) + 2, 2);
        put_line(VGA, OFFSET_X[player] + (3 * GRID_WIDTH) + 3 + (2 * 4) + 2, OFFSET_Y - 3, 2, (3 * 2 * 4) + (4 * 1) + (2 * 2));
        put_line(VGA, OFFSET_X[player] + (3 * GRID_WIDTH) + 3, OFFSET_Y + (3 * 2 * 4) + (3 * 1), (2 * 4) + 2, 2);
    }
}

//Contributed by Both
void loop(){    // game loop
    // Change direction to 1 = output so the gpio can be written to
    // *direction = 0x03FF;

    // poll inputs
    int value = read_gpio();
    print("Value in game loop: ");
    print_hex32(value);
    print("\n");
    int rot1 = value & 0x0001;
    int mv_r1 = value & 0x0002;
    int mv_l1 = value & 0x0004;
    int down1 = value & 0x0008;
    int hold1 = value & 0x0010;
    int rot2 = value & 0x0020;
    int mv_r2 = value & 0x0040;
    int mv_l2 = value & 0x0080;
    int down2 = value & 0x0100;
    int hold2 = value & 0x0200;

    // Player 1
    if(!player_list[0].lost){
        // Change direction to 0 = input so the gpio can be read
        // *direction = 0x00;

        if (rot1){
            rotate(0);
        }
        if(mv_r1){
            mov_hor(0, 1);
        }
        if(mv_l1){
            mov_hor(0, -1);
        }
        if(down1){
            hard_down(0);
        }
        if(hold1 && player_list[0].hold_available){
            hold_func(0);
        }

        // Reset direction to 1 = output so the gpio can be written to
        // *direction = 0x03FF;
    }

    // Player 2
    if(multiplayer)
    {
        if(!player_list[1].lost){
            // Change direction to 0 = input so the gpio can be read
            // *direction = 0x00;

            if (rot2){
                rotate(1);
            }
            if(mv_r2){
                mov_hor(1, 1);
            }
            if(mv_l2){
                mov_hor(1, -1);
            }
            if(down2){
                hard_down(1);
            }
            if(hold2 && player_list[1].hold_available){
                player_list[1].hold_available = false;
                hold_func(1);
            }

            // Reset direction to 1 = output so the gpio can be written to
            // *direction = 0x03FF;
        }
    }
}