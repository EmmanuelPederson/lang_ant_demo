import sys
import lang_ant
from langtons_ant import Runner, MultiAntRunner, StrBasedRule

num_ants = 5
rule_str = 'lrr'

def run_lang_ant_py(iterations):
    return Runner().run(StrBasedRule(rule_str), iterations=iterations)

def run_lang_ant_c(iterations):
    return lang_ant.sim_lang_ant(rule_str, iterations)

def run_lang_ant_colony_py(iterations):
    pos_list = [(x, x) for x in range(num_ants)]
    dir_list = [0 for _ in range(num_ants)]
    MultiAntRunner(StrBasedRule(rule_str)).run(iterations, pos_list, dir_list, dict())
    return pos_list

def run_lang_ant_colony_c(iterations):
    return lang_ant.sim_lang_ant_colony(rule_str, iterations, num_ants)

def face_off(iterations):
    for i in range(iterations):
        c_res = run_lang_ant_c(i)
        py_res = run_lang_ant_py(i)
        if py_res != c_res:
            print('failed at {}'.format(i))
            print('c: {}'.format(c_res))
            print('py: {}'.format(py_res))
            return
    print('No differences')

def face_off_colony(iterations):
    for i in range(iterations):
        c_res = run_lang_ant_colony_c(i)
        py_res = run_lang_ant_colony_py(i)
        if py_res != c_res:
            print('failed at {}'.format(i))
            print('colony c: {}'.format(c_res))
            print('colony py: {}'.format(py_res))
            return
    print('No differences')

if __name__ == '__main__':
    n = int(sys.argv[1])
    face_off(n)
    face_off_colony(n)

