#include <stdio.h>
#include "lang_ant_types.h"


State new_state(State state, StateDirRule *state_dir_rule){
    return (State)(((int)state + 1) % state_dir_rule->state_dir_cnt); 
}

Dir new_dir(State state, Dir direction, StateDirRule *state_dir_rule){
    return (Dir)((direction + state_dir_rule->state_dir_arr[(int)state]) % 4);
}
