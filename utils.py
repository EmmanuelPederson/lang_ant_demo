from itertools import product
from math import sqrt
from typing import Iterator

from langtons_ant import DictBasedRule, LEFT, RIGHT, Pos


# Utilities for analysis of langton's ant simulation


def dist(pos1: Pos, pos2: Pos) -> float:
    """
    Return the distance between two positions
    """
    return sqrt(sum((y - x) ** 2 for x, y in zip(pos1, pos2)))


def get_all_rules(states: int) -> Iterator[str]:
    """
    Return all rules that can be constructed for a given number of states
    """
    return map(''.join, product('lr', repeat=states))

