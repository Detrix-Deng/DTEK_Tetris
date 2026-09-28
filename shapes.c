//Contains only shapes

#include <stddef.h>

struct sprite_shape {
    char sprite_id;
    char matrix[4][4];
};

struct sprite_shape sprite_shapes[] = {
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
    {4, {//I
            {1,0,0,0},
            {1,0,0,0},
            {1,0,0,0},
            {1,0,0,0},
        }
    }
};

struct sprite{
    char sprite_shape;
    char rotation;
    int x;
    int y;
};

int* get_shape_bottom(){
    // Logic to find 
    int *arr;
    return arr;
}
