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
#define DEFAULT_STATE 0
const XYPos DIR_POS_VALS[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};


static void add_state_pos(HashTable *htable, XYPos pos, PosState *pos_state){
    PosState *stored_pos_state;
    stored_pos_state = (PosState *)search_table(htable, pos, (void *)pos_state, ENTER);
    if(stored_pos_state == NULL){
        fprintf(stderr, "Failed to enter state into hash table.\n");
        exit(EXIT_FAILURE);
    }
    stored_pos_state->state = pos_state->state;
}

static PosState *get_state_pos(HashTable *htable, XYPos pos){
    PosState *stored_pos_state;
    stored_pos_state = (PosState *)search_table(htable, pos, NULL, SEARCH);
    if(stored_pos_state != NULL){
        return stored_pos_state;
    }else{
        return NULL;
    }
}

static XYPos get_new_position(XYPos pos, Dir dir){
    XYPos change = DIR_POS_VALS[dir];
    XYPos new_pos = {pos.x + change.x, pos.y + change.y};
    return new_pos;
}

static PosState *new_pos_state(XYPos pos, State state){
    PosState *pos_state;
    pos_state = malloc(sizeof(PosState));
    if(pos_state == NULL){
        fprintf(stderr, "Could not allocate memory for PosState");
        exit(EXIT_FAILURE);
    }else{
        pos_state->state = state;
        return pos_state;
    }
}

static PosStateList *new_pos_state_list_item(PosState *pos_state){
    PosStateList *pos_state_list;
    pos_state_list = malloc(sizeof(PosStateList));
    if(pos_state_list == NULL){
        fprintf(stderr, "Could not allocate memory for PosStateList");
        exit(EXIT_FAILURE);
    }else{
        pos_state_list->pos_state = pos_state;
        pos_state_list->next = NULL;
        return pos_state_list;
    }
}

static void free_pos_state_list(PosStateList *pos_state_list){
    PosStateList *cur, *temp;
    cur = pos_state_list;
    while (cur){
        temp = cur->next;
        free(cur->pos_state);
        free(cur); 
        cur = temp;
    }
}

static HashTable *check_htable(HashTable *htable){
    HashTable *new_htable;
    if ((htable->size * 100) / htable->capacity > 90){
        new_htable = rehash(htable, (htable->capacity) * 2); 
        free_xypos_hash(htable);
        return new_htable;
    }else{
        return htable;
    }
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
    HashTable *htable;
    PosState *cur_pos_state, **pos_states;
    PosStateList *ps_list_head, *ps_list_last;
    State *states;
    XYPos origin = {0, 0};
    XYPos pos;
    
    htable = create_xypos_hash(INITIAL_CAP);

    cur_pos_state = new_pos_state(origin, DEFAULT_STATE);
    add_state_pos(htable, origin, cur_pos_state);
    ps_list_head = new_pos_state_list_item(cur_pos_state);
    ps_list_last = ps_list_head;

    states = calloc(sizeof(State), num_ants);
    pos_states = calloc(sizeof(PosState *), num_ants);
    
    for (int i = 0; i < n; i++){

        /* Read the states from the ants before updating their positions */
        for (int j = 0; j < num_ants; j++){
            pos = pos_arr[j];
            cur_pos_state = get_state_pos(htable, pos);
            if (cur_pos_state == NULL){
                cur_pos_state = new_pos_state(pos, DEFAULT_STATE);
                add_state_pos(htable, pos, cur_pos_state);
                ps_list_last->next = new_pos_state_list_item(cur_pos_state);
                ps_list_last = ps_list_last->next;
            }
            pos_states[j] = cur_pos_state;
            states[j] = cur_pos_state->state;
        }

        langtons_ant_one_step(rule, num_ants, pos_arr, dir_arr, states);

        /* Update the previous pos states to their new values */
        for (int j = 0; j < num_ants; j++){
            pos_states[j]->state = states[j];
        }

        htable = check_htable(htable);
    }
    free(states);
    free(pos_states);
    free_pos_state_list(ps_list_head);
    free_xypos_hash(htable);
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

