from itertools import product
from math import sqrt
from typing import Iterator

from langtons_ant import DictBasedRule, LEFT, RIGHT, Pos


# Utilities for analysis of langton's ant simulation


def dist(pos1: Pos, pos2: Pos) -> float:
    return sqrt(sum((y - x) ** 2 for x, y in zip(pos1, pos2)))


def get_all_rules(states: int) -> Iterator[DictBasedRule]:
    """
    Return all the rules that can be constructed for a given number of states
    """
    return (DictBasedRule(dict(enumerate(p))) for p in
            product((LEFT, RIGHT), repeat=states))

