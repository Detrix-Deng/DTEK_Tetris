//Contains only shapes

#include <stddef.h>

struct sprite {
    char sprite_id;
    char rotation;
    int x;
    int y;
};

int* get_shape_bottom(){
    // Logic to find 
    int *arr;
    return arr;
}
char sprite_shape[] = {
    {0, {//L
            {1,0,0,0,},
            {1,0,0,0,},
            {1,1,0,0,},
            {0,0,0,0,},
        }
    },
    {1, {//T
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        }
    },
    {2, {//Cube
            {1,1,0,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0},
        }
    },
    {3, {//Z
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0},
        }
    },
    {4, {//I{
        {1,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        {1,0,0,0},
        }
    }
};
