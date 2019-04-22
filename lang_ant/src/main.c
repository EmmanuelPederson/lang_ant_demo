#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lang_ant_types.h"
#include "pos_hash.h"
#include "lang_rule.h"


int main(int argc, char** argv){
    int iterations = 10000;
    char rule_str[MAX_RULES];
    StateDirRule *state_dir_rule;
    XYPos final_pos;

    if(argc == 3){
        strncpy(rule_str, argv[1], MAX_RULES);
        sscanf(argv[2], "%d", &iterations);
    }else{
        printf("Rule string and iteration count required: ie: ./a.out lrr 100\n");
        return -1;
    }

    printf("Simulating for %d iterations\n", iterations);
    state_dir_rule = make_rule_from_str(rule_str);
    final_pos = langtons_ant(state_dir_rule, iterations);
    free(state_dir_rule);
    printf("Final pos: (%d, %d)\n", final_pos.x, final_pos.y);
}

