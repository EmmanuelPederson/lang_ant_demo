from typing import Dict

from langtons_ant import Rule, Pos, Dir, State, DIRECTIONS, MultiAntRunner


class LangAntSim(object):

    def __init__(
            self, lang_rule: Rule,
            initial_pos_list=[(0, 0)],
            initial_dir_list=[0],
            state_map=None
    ) -> None:
        assert len(initial_pos_list) == len(initial_dir_list)

        self.state_map = state_map
        if self.state_map is None:
            self.state_map = dict()

        # Keep track of changes since get_coords was last called
        self.updated_state_map = self.state_map.copy()

        self.lang_rule = lang_rule
        self.cur_pos_list = initial_pos_list
        self.cur_dir_list = initial_dir_list

    @staticmethod
    def get_new_position(pos: Pos, direction: Dir) -> Pos:
        return tuple((p + d for p, d in zip(pos, DIRECTIONS[direction])))

    def advance(self, steps: int) -> None:
        self.updated_state_map = MultiAntRunner(self.lang_rule).run(
            steps, self.cur_pos_list, self.cur_dir_list, self.state_map
        )

    def get_coords(self) -> Dict[Pos, State]:
        to_return = self.updated_state_map
        self.updated_state_map = dict()
        return to_return
