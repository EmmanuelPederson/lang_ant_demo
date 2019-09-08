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

/* Return the position and states after 1 iteration*/
State *langtons_ant_one_step(
    StateDirRule *rule, 
    int num_ants,
    XYPos *pos_arr, 
    Dir *dir_arr, 
    HashTable *htable
){
    XYPos cur_pos;
    Dir cur_dir;
    State cur_state;
    PosState *cur_pos_state;
    State *state_updates;
    state_updates = calloc(sizeof(State), num_ants);

    for (int i = 0; i < num_ants; i++){
        cur_pos = pos_arr[i];
        cur_dir = dir_arr[i];

        /* Move one space forward */
        cur_pos = get_new_position(cur_pos, cur_dir);

        /* Get state of new space */
        cur_pos_state = get_state_pos(htable, cur_pos);
        if (cur_pos_state){
            cur_state = cur_pos_state->state;
        } else {
            cur_state = DEFAULT_STATE;
        }

        /* Turn and update space state */
        cur_dir = new_dir(cur_state, cur_dir, rule);
        cur_state = new_state(cur_state, rule);
        state_updates[i] = cur_state;

        pos_arr[i] = cur_pos;
        dir_arr[i] = cur_dir;
    }
    return state_updates;
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
    PosState *cur_pos_state;
    PosStateList *ps_list_head, *ps_list_last;
    State *state_updates;
    State new_state;
    XYPos origin = {0, 0};
    XYPos new_pos;
    
    htable = create_xypos_hash(INITIAL_CAP);

    // Set to 1 to match python. C moves then updates state
    // while python updates and them moves.
    cur_pos_state = new_pos_state(origin, 1);
    add_state_pos(htable, origin, cur_pos_state);
    ps_list_head = new_pos_state_list_item(cur_pos_state);
    ps_list_last = ps_list_head;

    for (int i = 0; i < n; i++){
        state_updates = langtons_ant_one_step(
            rule, num_ants, pos_arr, dir_arr, htable
        );

        for (int j = 0; j < num_ants; j++){
            new_state = state_updates[j];
            new_pos = pos_arr[j];
            cur_pos_state = get_state_pos(htable, new_pos);
            if (cur_pos_state == NULL){
                cur_pos_state = new_pos_state(new_pos, new_state);
                add_state_pos(htable, new_pos, cur_pos_state);
                ps_list_last->next = new_pos_state_list_item(cur_pos_state);
                ps_list_last = ps_list_last->next;
            } else {
                cur_pos_state->state = new_state;
            }
        }
        htable = check_htable(htable);
    }
    free_pos_state_list(ps_list_head);
    free_xypos_hash(htable);
}

XYPos langtons_ant(StateDirRule *state_dir_rule, int iterations){
    XYPos initial_pos = {0, 0};
    int num_ants = 1;
    XYPos *pos_arr;
    Dir *dir_arr;
    
    pos_arr = calloc(sizeof(XYPos), num_ants);
    dir_arr = calloc(sizeof(Dir), num_ants);
    for (int i = 0; i < num_ants; i++){
        pos_arr[i] = initial_pos;
        dir_arr[i] = UP;
    }

    langtons_ant_colony(state_dir_rule, iterations, num_ants, pos_arr, dir_arr);

    // todo: free pos_arr/dir_arr
    return pos_arr[0];
}

