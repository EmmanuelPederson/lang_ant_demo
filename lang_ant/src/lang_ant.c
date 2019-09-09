/*
 * Langton's Ant program will calculate the final position
 * of an ant's movement after n iterations. Starting point
 * is (0, 0).
 * 
 * Emmanuel Pederson 23 February 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lang_ant_types.h"
#include "lang_rule.h"
#include "pos_hash.h"

#define INITIAL_CAP 1024
const XYPos DIR_POS_VALS[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};


static XYPos get_new_position(XYPos pos, Dir dir){
    XYPos change = DIR_POS_VALS[dir];
    XYPos new_pos = {pos.x + change.x, pos.y + change.y};
    return new_pos;
}

/* 
 * Return the position and states after 1 iteration
 * 
 * Positions updated to new positions
 * Directions updated to new directions
 * States AT OLD POSITIONS updated to new states
 */ 
void langtons_ant_one_step(
    StateDirRule *rule, 
    int num_ants,
    XYPos *pos_arr, 
    Dir *dir_arr, 
    State *states
){
    XYPos cur_pos;
    Dir cur_dir;
    State cur_state;

    for (int i = 0; i < num_ants; i++){
        cur_pos = pos_arr[i];
        cur_dir = dir_arr[i];
        cur_state = states[i];

        cur_dir = new_dir(cur_state, cur_dir, rule);
        cur_state = new_state(cur_state, rule);
        cur_pos = get_new_position(cur_pos, cur_dir);

        pos_arr[i] = cur_pos;
        dir_arr[i] = cur_dir;
        states[i] = cur_state;
    }
}

/* Return the updated pos states after n iterations. */
void langtons_ant_colony(
    StateDirRule *rule, 
    int n,
    int num_ants,
    XYPos *pos_arr, 
    Dir *dir_arr
){
    MemHashTable *mem_htable;
    PosState *cur_pos_state, **pos_states;
    State *states;
    XYPos pos;
    
    mem_htable = create_mem_htable(INITIAL_CAP);

    states = calloc(sizeof(State), num_ants);
    pos_states = calloc(sizeof(PosState *), num_ants);
    
    for (int i = 0; i < n; i++){

        /* Read the states from the ants before updating their positions */
        for (int j = 0; j < num_ants; j++){
            pos = pos_arr[j];
            cur_pos_state = get_state_pos(mem_htable, pos);
            pos_states[j] = cur_pos_state;
            states[j] = cur_pos_state->state;
        }

        langtons_ant_one_step(rule, num_ants, pos_arr, dir_arr, states);

        /* Update the previous pos states to their new values */
        for (int j = 0; j < num_ants; j++){
            pos_states[j]->state = states[j];
        }
    }
    free(states);
    free(pos_states);
    free_mem_htable(mem_htable);
}

XYPos langtons_ant(StateDirRule *state_dir_rule, int iterations){
    XYPos initial_pos = {0, 0};
    int num_ants = 1;
    XYPos *pos_arr, to_return;
    Dir *dir_arr;
    
    pos_arr = calloc(sizeof(XYPos), num_ants);
    dir_arr = calloc(sizeof(Dir), num_ants);
    for (int i = 0; i < num_ants; i++){
        pos_arr[i] = initial_pos;
        dir_arr[i] = RIGHT;
    }

    langtons_ant_colony(state_dir_rule, iterations, num_ants, pos_arr, dir_arr);
    to_return = pos_arr[0];

    free(pos_arr);
    free(dir_arr);
    return to_return;
}

