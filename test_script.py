import sys
import timeit
from langtons_ant import Runner, StrBasedRule
import lang_ant
from profile_decorators import profiler, timer, malloc_tracer


def run_lang_ant_py(iterations):
    return Runner().run(StrBasedRule('lrr'), iterations=iterations)


def run_lang_ant_c(iterations):
    return lang_ant.sim_lang_ant(bytes([1, 0, 0]), iterations)


if __name__ == '__main__':
    c = int(sys.argv[1])
    n = int(sys.argv[2])
    print('{} executions with n: {}'.format(c, n))
    print('C lang ant: {} took {:.3f} seconds'.format(
            run_lang_ant_c(n),
            timeit.timeit('run_lang_ant_c(n)', number=c, globals=globals()),
    ))
    print('Python lang ant: {} took {:.3f} seconds'.format(
            run_lang_ant_py(n),
            timeit.timeit('run_lang_ant_py(n)', number=c, globals=globals()),
    ))
