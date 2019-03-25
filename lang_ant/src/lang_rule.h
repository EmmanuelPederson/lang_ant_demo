/*
 * Defines the rules for langton's ant.
 * Given a state, what is the next state
 * Given a state and a direction, what should be the new direction
 */
#ifndef LANG_ANT_RULES_HEADER
#define LANG_ANT_RULES_HEADER

#include "lang_ant_types.h"

State new_state(State state, StateDirRule *state_dir_rule);

Dir new_dir(State state, Dir direction, StateDirRule *state_dir_rule);

#endif
