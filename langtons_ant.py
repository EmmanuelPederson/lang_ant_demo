from typing import Tuple, Dict, List

DIRECTIONS = ((1, 0), (0, 1), (-1, 0), (0, -1))
LEFT = 1
RIGHT = -1
DEFAULT_STATE = 0
Pos = Tuple[int, int]
State = int
Dir = int


class Rule(object):

    def get_new_state(self, state: State) -> State:
        pass

    def get_new_direction(self, state: State, direction: Dir) -> Dir:
        pass


class DictBasedRule(Rule):

    def __init__(self, update_dict):
        assert not set(update_dict.values()) - {LEFT, RIGHT}
        self.update_dict = update_dict

    def get_new_state(self, state: State) -> State:
        return (state + 1) % len(self.update_dict.keys())

    def get_new_direction(self, state: State, direction: Dir) -> Dir:
        return (direction + self.update_dict[state]) % len(DIRECTIONS)

    def __str__(self):
        return str(self.update_dict)


class StrBasedRule(Rule):

    def __init__(self, rule_str: str):
        """
        Creates a rule out of a rule string of 'L' or 'R' chars

        ie: 'lrr' -> Rule with three states and actions Left, Right, Right
        """
        self.rule_str = rule_str
        self.dict_based_rule = DictBasedRule(
            dict(enumerate(map(self.convert, rule_str)))
        )

    @staticmethod
    def convert(rule_char: str) -> int:
        if rule_char.upper() == 'L':
            return 1
        elif rule_char.upper() == 'R':
            return -1
        else:
            raise ValueError(
                'Cannot convert char "{}" to movement rule'.format(rule_char)
            )

    def get_new_state(self, state: State) -> State:
        return self.dict_based_rule.get_new_state(state)

    def get_new_direction(self, state: State, direction: Dir) -> Dir:
        return self.dict_based_rule.get_new_direction(state, direction)

    def __str__(self):
        return str(self.rule_str.upper())


class Runner(object):

    @staticmethod
    def get_new_position(pos: Pos, direction: Dir) -> Pos:
        new_dir = DIRECTIONS[direction]
        return pos[0] + new_dir[0], pos[1] + new_dir[1]

    @staticmethod
    def get_state(pos: Pos, state_dict: Dict[Pos, State]) -> State:
        return state_dict.get(pos, DEFAULT_STATE)

    def run(self, rule: Rule, iterations: int) -> float:
        initial_pos = (0, 0)
        state_dict = {initial_pos: DEFAULT_STATE}

        cur_dir = 0
        pos = initial_pos
        for _ in range(iterations):
            cur_state = self.get_state(pos, state_dict)
            new_state = rule.get_new_state(cur_state)
            state_dict[pos] = new_state
            cur_dir = rule.get_new_direction(cur_state, cur_dir)
            pos = self.get_new_position(pos, cur_dir)
        return pos


class MultiAntRunner(object):

    def __init__(self, rule:Rule):
        self.rule = rule

    @staticmethod
    def get_new_position(pos: Pos, direction: Dir) -> Pos:
        new_dir = DIRECTIONS[direction]
        return pos[0] + new_dir[0], pos[1] + new_dir[1]

    @staticmethod
    def get_state(pos: Pos, state_dict: Dict[Pos, State]) -> State:
        return state_dict.get(pos, DEFAULT_STATE)

    def run(
            self,
            iterations: int,
            pos_list: List[Pos],
            dir_list: List[Dir],
            state_dict: Dict[Pos, State]
    ):
        num_ants = len(pos_list)
        all_updates = dict()
        for _ in range(iterations):
            step_updates = dict()
            for i in range(num_ants):
                cur_pos = pos_list[i]
                cur_dir = dir_list[i]
                cur_state = self.get_state(cur_pos, state_dict)
                new_state = self.rule.get_new_state(cur_state)
                step_updates[cur_pos] = new_state
                cur_dir = self.rule.get_new_direction(cur_state, cur_dir)
                cur_pos = self.get_new_position(cur_pos, cur_dir)
                pos_list[i] = cur_pos
                dir_list[i] = cur_dir
            state_dict.update(step_updates)
            all_updates.update(step_updates)
        return all_updates
