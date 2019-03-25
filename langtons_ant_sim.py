from typing import Dict

from langtons_ant import Rule, Pos, Dir, \
    State, DEFAULT_STATE, DIRECTIONS


class LangAntSim(object):

    def __init__(
            self, lang_rule: Rule,
            initial_pos=(0, 0),
            initial_direction=0,
            state_map=None
    ) -> None:

        self.state_map = state_map
        if self.state_map is None:
            self.state_map = dict()

        # Keep track of changes since get_coords was last called
        self.updated_state_map = self.state_map.copy()

        self.lang_rule = lang_rule
        self.cur_pos = initial_pos
        self.cur_dir = initial_direction

    @staticmethod
    def get_new_position(pos: Pos, direction: Dir) -> Pos:
        return tuple((p + d for p, d in zip(pos, DIRECTIONS[direction])))

    def advance(self, steps: int) -> None:
        for _ in range(steps):
            cur_state = self.state_map.get(self.cur_pos, DEFAULT_STATE)
            self.state_map[self.cur_pos] = self.lang_rule.get_new_state(
                cur_state)
            self.updated_state_map[self.cur_pos] = self.lang_rule.get_new_state(
                cur_state)
            self.cur_dir = self.lang_rule.get_new_direction(cur_state,
                                                            self.cur_dir)
            self.cur_pos = self.get_new_position(self.cur_pos, self.cur_dir)

    def get_coords(self) -> Dict[Pos, State]:
        to_return = self.updated_state_map
        self.updated_state_map = dict()
        return to_return
