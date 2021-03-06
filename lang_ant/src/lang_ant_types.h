/*
 * Langton's Ant program will calculate the distance
 * that an "ant" will travel from a starting point. 
 * 
 * Emmanuel Pederson 23 February 2019
 */

#ifndef LANG_ANT_TYPES_HEADER
#define LANG_ANT_TYPES_HEADER
#define MAX_RULES 20

typedef unsigned char State;

typedef struct{
    int x;
    int y;
} XYPos;

typedef struct{
    State state;
} PosState;

typedef enum{
    RIGHT = 0,
    UP    = 1,
    LEFT  = 2,
    DOWN  = 3
} Dir;

// state_dir_arr is an array where the position is the state and the
// value at the position is the direction to turn: 1 or -1 for left/right
typedef struct {
    int state_dir_cnt;
    int state_dir_arr[MAX_RULES];  
} StateDirRule; 

typedef struct PosStateList PosStateList;
struct PosStateList{
    PosState *pos_state;
    PosStateList *next;
};

/* Simulate single ant starting at the origin */
XYPos langtons_ant(StateDirRule *state_dir_rule, int iterations);

/* Simulate multiple ants */
void langtons_ant_colony(
    StateDirRule *state_dir_rule, 
    int iterations, 
    int num_ants, 
    XYPos *pos_arr,
    Dir *dir_arr
);

#endif
