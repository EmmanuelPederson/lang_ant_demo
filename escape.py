import sys
import lang_ant
from langtons_ant import Runner, StrBasedRule
from math import sqrt
from itertools import product
from utils import get_all_rules, dist

def opposite(char):
    if char == 'l':
        return 'r'
    if char == 'r':
        return 'l'
    raise ValueError('Unknown char {}'.format(char))

def inverse_rule(rule_str):
    return ''.join(map(opposite, rule_str))

def pattern_repeats(pattern, rule_str):
    if pattern == rule_str:
        return True
    else:
        if pattern == rule_str[:len(pattern)]:
            return pattern_repeats(pattern, rule_str[len(pattern):])
        else:
            return False

def get_repeating_pattern(rule_str):
    rule_len = len(rule_str)
    for i in range(1, rule_len//2 + 1):
        if (rule_len % i) == 0:
            pattern = rule_str[:i]
            if pattern_repeats(pattern, rule_str):
                return pattern
    return None

def print_end_distances(iterations, max_state_cnt):

    seen = set()

    for i in range(max_state_cnt + 1):
        for rule_str in get_all_rules(i):
            if inverse_rule(rule_str) in seen:
                seen.add(rule_str)
                print('skipping rule: {}, found inverse'.format(rule_str))
            elif get_repeating_pattern(rule_str) in seen:
                seen.add(rule_str)
                print('skipping rule: {}, repeating pattern'.format(rule_str))
            else:
                seen.add(rule_str)
                end_pos = lang_ant.sim_lang_ant(rule_str, iterations)
                print('{},{}'.format(rule_str, dist((0, 0), end_pos)))

if __name__ == '__main__':
    n = int(sys.argv[1])
    max_state_cnt = int(sys.argv[2])
    print_end_distances(n, max_state_cnt)

