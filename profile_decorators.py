import cProfile
import datetime
import functools
import tracemalloc


def timer(function):
    @functools.wraps(function)
    def _timer(*args, **kwargs):
        start = datetime.datetime.now()
        try:
            return function(*args, **kwargs)
        finally:
            end = datetime.datetime.now()
            print('{}: {}'.format(function.__name__, end - start))
    return _timer


def profiler(function):
    @functools.wraps(function)
    def _profiler(*args, **kwargs):
        profiler = cProfile.Profile()
        try:
            profiler.enable()
            return function(*args, **kwargs)
        finally:
            profiler.disable()
            profiler.print_stats()
    return _profiler

def malloc_tracer(function):
    @functools.wraps(function)
    def _malloc_tracer(*args, **kwargs):
        tracemalloc.start()
        try:
            return function(*args, **kwargs)
        finally:
            snapshot = tracemalloc.take_snapshot()
            for statistic in snapshot.statistics('lineno'):
                print(statistic)
            tracemalloc.stop()
    return _malloc_tracer
            
