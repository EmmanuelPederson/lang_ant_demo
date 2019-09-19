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
#include <pthread.h>

#define INITIAL_CAP 1024
#define THREADS 8

void *thread(void *vargp);

const XYPos DIR_POS_VALS[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};


typedef struct{
    StateDirRule *rule;
    int num_ants;
    XYPos *pos_arr;
    Dir *dir_arr;
    PosState **pos_states;
    State *states;
    MemHashTable *mem_htable;
} StepState;

typedef struct{
    int start_index;
    int end_index;
    StepState *step_state;
} ThreadStep;

static XYPos get_new_position(XYPos pos, Dir dir){
    XYPos change = DIR_POS_VALS[dir];
    XYPos new_pos = {pos.x + change.x, pos.y + change.y};
    return new_pos;
}

static ThreadStep **make_thread_steps(StepState *step_state, int num_threads){
    ThreadStep **thread_steps = calloc(sizeof(ThreadStep*), num_threads);
    ThreadStep *thread_step;
    int num_ants = step_state->num_ants;
    int ants_per_thread = (num_ants + 1) / num_threads;
    int start_index, end_index; 
    
    for (int i = 0; i < num_threads; i++){
        start_index = i * ants_per_thread;
        end_index = start_index + ants_per_thread;
        if (end_index > num_ants)
            end_index = num_ants;

        thread_step = malloc(sizeof(ThreadStep));
        thread_step->start_index = start_index;
        thread_step->end_index = end_index;
        thread_step->step_state = step_state;
        thread_steps[i] = thread_step;
    }
    return thread_steps;
}

static void free_thread_steps(ThreadStep **thread_steps, int num_threads){
    for (int i = 0; i < num_threads; i++){
        free(thread_steps[i]);
    }
    free(thread_steps);
}

/* 
 * Return the position and states after 1 iteration
 * 
 * Positions updated to new positions
 * Directions updated to new directions
 * States AT OLD POSITIONS updated to new states
 */ 
static void langtons_ant_one_step(ThreadStep *thread_step){
    StepState *step_state = thread_step->step_state;
    StateDirRule *rule = step_state->rule; 
    XYPos *pos_arr = step_state->pos_arr; 
    Dir *dir_arr = step_state->dir_arr;
    State *states = step_state->states;
    MemHashTable *mem_htable = step_state->mem_htable;
    XYPos cur_pos;
    Dir cur_dir;
    State cur_state;
    PosState *cur_pos_state;

    for (int i = thread_step->start_index; i < thread_step->end_index; i++){
        cur_pos = pos_arr[i];
        cur_dir = dir_arr[i];

        cur_pos_state = get_state_pos(mem_htable, cur_pos);
        step_state->pos_states[i] = cur_pos_state;
        cur_state = cur_pos_state->state;

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
    StepState *step_state;
    ThreadStep **thread_steps;
    pthread_t tid_arr[THREADS];
    
    mem_htable = create_mem_htable(INITIAL_CAP);

    pos_states = calloc(sizeof(PosState *), num_ants);
    states = calloc(sizeof(State), num_ants);

    /* Read the states from the ants before updating their positions */
    for (int j = 0; j < num_ants; j++){
        pos = pos_arr[j];
        cur_pos_state = get_state_pos(mem_htable, pos);
        pos_states[j] = cur_pos_state;
        states[j] = cur_pos_state->state;
    }

    /* Encapsulate the state in a step */
    step_state = malloc(sizeof(StepState));
    step_state->rule = rule;
    step_state->num_ants = num_ants;
    step_state->pos_arr = pos_arr;
    step_state->dir_arr = dir_arr;
    step_state->pos_states = pos_states;
    step_state->states = states;
    step_state->mem_htable = mem_htable;
    thread_steps = make_thread_steps(step_state, THREADS);

    for (int i = 0; i < n; i++){
        for (int j = 0; j < THREADS; j++){
            pthread_create(&tid_arr[j], NULL, thread, thread_steps[j]);
        }
        for (int j = 0; j < THREADS; j++){
            pthread_join(tid_arr[j], NULL);
        }
        /*
        for (int j = 0; j < THREADS; j++){
            langtons_ant_one_step(thread_steps[j]);
        }
        */

        /* Update the previous pos states to their new values */
        for (int j = 0; j < num_ants; j++){
            pos_states[j]->state = states[j];
        }
    }
    free(step_state);
    free_thread_steps(thread_steps, THREADS);
    free(states);
    free(pos_states);
    free_mem_htable(mem_htable);
}

XYPos langtons_ant(StateDirRule *state_dir_rule, int iterations){
    XYPos initial_pos = {0, 0};
    int num_ants = 100000;
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

void *thread(void *vargp){
    langtons_ant_one_step(vargp);
    return NULL;
}

