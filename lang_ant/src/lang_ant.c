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

XYPos langtons_ant(StateDirRule state_dir_rule, int iterations){
    PosState *cur_pos_state;
    HashTable *htable;
    PosStateList *ps_list_head, *ps_list_last;
    Dir cur_dir = RIGHT;
    XYPos initial_pos = {0, 0};
    XYPos cur_pos;

    htable = create_xypos_hash(INITIAL_CAP);
    cur_pos = initial_pos;
    cur_pos_state = new_pos_state(cur_pos, DEFAULT_STATE);
    add_state_pos(htable, cur_pos, cur_pos_state);
    ps_list_head = new_pos_state_list_item(cur_pos_state);
    ps_list_last = ps_list_head;

    for (int i = 0; i < iterations; i++){
        cur_pos_state = get_state_pos(htable, cur_pos);
        if (cur_pos_state == NULL){
            cur_pos_state = new_pos_state(cur_pos, DEFAULT_STATE);
            add_state_pos(htable, cur_pos, cur_pos_state);
            ps_list_last->next = new_pos_state_list_item(cur_pos_state);
            ps_list_last = ps_list_last->next;
        }
        cur_dir = new_dir(cur_pos_state->state, cur_dir, &state_dir_rule);
        cur_pos_state->state = new_state(cur_pos_state->state, &state_dir_rule);
        cur_pos = get_new_position(cur_pos, cur_dir);
        htable = check_htable(htable);
    }
    free_pos_state_list(ps_list_head);
    free_xypos_hash(htable);
    return cur_pos;
}
