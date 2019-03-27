#include <stdio.h>
#include "lang_ant_types.h"
#include "pos_hash.h"


int main(int argc, char** argv){
    int iterations = 10000;
    if(argc == 2){
        sscanf(argv[1], "%d", &iterations);
    }
    printf("Simulating for %d iterations\n", iterations);
    XYPos final_pos;
    StateDirRule state_dir_rule = {3, {1, -1, -1}};

    printf("size of int: %lu\n", sizeof(int));
    printf("size of PosState: %lu\n", sizeof(PosState));
    printf("size of hash elem: %lu\n", sizeof(HashElem));
    printf("size of void*: %lu\n", sizeof(void*));

    final_pos = langtons_ant(state_dir_rule, iterations);
    printf("Final pos: (%d, %d)\n", final_pos.x, final_pos.y);
}

