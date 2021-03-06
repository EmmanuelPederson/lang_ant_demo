import sys
import timeit
from langtons_ant import Runner, StrBasedRule
import lang_ant

def run_lang_ant_py(iterations):
    return Runner().run(StrBasedRule('lrr'), iterations=iterations)

def run_lang_ant_c(iterations):
    return lang_ant.sim_lang_ant('lrr', iterations)

if __name__ == '__main__':
    c = int(sys.argv[1])
    n = int(sys.argv[2])
    step_factor = 20
    print('Program\tCount\tIterations\tTime(s)')
    for i in range(step_factor):
        print('C_lang_ant\t%d\t%d\t%.5f' % (c, i * n, timeit.timeit('run_lang_ant_c(n * i)', number=c, globals=globals())))
    for i in range(step_factor):
        print('Py_lang_ant\t%d\t%d\t%.5f\t' % (c, i * n, timeit.timeit('run_lang_ant_py(n * i)', number=c, globals=globals())))
