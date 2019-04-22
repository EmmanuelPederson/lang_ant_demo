#include <stdio.h>
#include <stdlib.h>
#include "lang_ant_types.h"


static int convert(char c){
    if (c == 'l')
        return 1;
    if (c == 'r')
        return -1;
    return 0;
}

StateDirRule *make_rule_from_str(const char *seq){
    StateDirRule *state_dir_rule;
    int converted;
    char cur;
    int cnt = 0;
    int i = 0;

    state_dir_rule = (StateDirRule *)malloc(sizeof(StateDirRule));

    cur = seq[0];
    while(cur != '\0' && i < MAX_RULES){
        cur = seq[i];
        converted = convert(cur);
        if (converted != 0){
            state_dir_rule->state_dir_arr[cnt] = converted;
            cnt++;
        }
        i++;
    }

    if (cnt)
        state_dir_rule->state_dir_cnt = cnt;
    else
        state_dir_rule->state_dir_cnt = 1;

    for (; cnt < MAX_RULES; cnt++){
        state_dir_rule->state_dir_arr[cnt] = 0;
    }

    return state_dir_rule;
}

State new_state(State state, StateDirRule *state_dir_rule){
    return (State)(((int)state + 1) % state_dir_rule->state_dir_cnt); 
}

Dir new_dir(State state, Dir direction, StateDirRule *state_dir_rule){
    return (Dir)((direction + state_dir_rule->state_dir_arr[(int)state]) % 4);
}
