import argparse

from langtons_ant import StrBasedRule
from langtons_ant_pygame import LangAntGame

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Run Langton's Ant Simulation")
    parser.add_argument(
        '--grid-size',
        type=int,
        default=65,
        help='Number of rows/cols, default = 65'
    )
    parser.add_argument(
        '--cell-size',
        type=int,
        default=10,
        help='Size of a single cell in pixels, default = 10'
    )
    parser.add_argument(
        '--clock-speed',
        type=int,
        default=60,
        help='Clock speed in frames/s, default = 60'
    )
    parser.add_argument(
        '--num-ants',
        type=int,
        default=1,
        help='Number of ants that will be simulated, default = 1'
    )
    parser.add_argument(
        '--lang-rule',
        type=str,
        default='lr',
        help='Rule for ant movement, default = lr'
    )
    args = parser.parse_args()

    LangAntGame(
        rows=args.grid_size,
        cols=args.grid_size,
        lang_rule=StrBasedRule(args.lang_rule),
        cell_height=args.cell_size,
        cell_width=args.cell_size,
        num_ants=args.num_ants,
        clock_speed=args.clock_speed,
    ).run()
