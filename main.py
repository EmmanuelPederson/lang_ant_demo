import argparse

from langtons_ant_pygame import LangAntGame
from langtons_ant import StrBasedRule

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Run Langton's Ant Simulation")
    parser.add_argument('--speed', nargs=1, default='mid',
                        help='Choose speed, slow, mid (default)')
    parser.add_argument('--grid_size', nargs=1, default=65,
                        help='Number of rows/cols, default = 65')
    parser.add_argument('--cell_size', nargs=1, default=10,
                        help='Size of a single cell in pixels, default = 10')
    parser.add_argument('--clock_speed', nargs=1, default=60,
                        help='Clock speed in frames/s, default = 60')
    parser.add_argument('--lang_rule', nargs=1, type=str, default=['lr'],
                        help='Clock speed in frames/s, default = 60')
    args = parser.parse_args()

    LangAntGame(
        rows=args.grid_size,
        cols=args.grid_size,
        lang_rule=StrBasedRule(''.join(args.lang_rule)),
        cell_height=args.cell_size,
        cell_width=args.cell_size,
        clock_speed=args.clock_speed,
    ).run()
