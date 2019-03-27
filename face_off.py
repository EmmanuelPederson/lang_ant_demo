import sys
import lang_ant
from langtons_ant import Runner, StrBasedRule

def run_lang_ant_py(iterations):
    return Runner().run(StrBasedRule('lrr'), iterations=iterations)

def run_lang_ant_c(iterations):
    return lang_ant.sim_lang_ant(bytes([1, 0, 0]), iterations)

def face_off(iterations):
    for i in range(iterations):
        c_res = run_lang_ant_c(i)
        py_res = run_lang_ant_py(i)
        if py_res != c_res:
            print('failed at {}'.format(i))
            return
    print('No differences')

if __name__ == '__main__':
    n = int(sys.argv[1])
    face_off(n)

