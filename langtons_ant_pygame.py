from typing import Dict, List, Tuple

import pygame

from langtons_ant_sim import LangAntSim
from langtons_ant import Rule, Pos, State, Dir
from random import randint

BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
PURPLE = (128, 0, 128)
BLUE = (0, 0, 255)
MAROON = (128, 0, 0)
TEAL = (0, 128, 128)
OLIVE = (128, 128, 0)
GREY = (105, 105, 105)


class LangAntGame(object):

    def __init__(
            self,
            rows: int,
            cols: int,
            lang_rule: Rule,
            num_ants=1,
            clock_speed=60,
            color_dict: Dict[int, Tuple] = None,
            cell_width: int = 10,
            cell_height: int = 10,
            cell_margin: int = 1,
    ):
        if not color_dict:
            color_dict = dict(
                enumerate([WHITE, GREEN, RED, PURPLE, BLUE, MAROON, TEAL, OLIVE, GREY])
            )

        self.rows = rows
        self.cols = cols
        self.clock_speed = clock_speed
        self.num_ants = num_ants
        self.step_rate = 1
        self.cell_width = cell_width
        self.cell_height = cell_height
        self.cell_margin = cell_margin
        self.lang_ant_sim = LangAntSim(
            lang_rule,
            initial_pos_list=self.make_rand_init_pos_list(),
            initial_dir_list=self.make_rand_dir_list()
        )
        self.grid = [[0] * self.cols for _ in range(self.rows)]
        self.color_dict = color_dict
        self.color_dict[-1] = BLACK

    def make_rand_init_pos_list(self) -> List[Pos]:
        """Returns random, central positions for ants."""
        half_rows = self.rows//20
        half_cols = self.cols//20

        def randx():
            return randint(-half_cols, half_cols)

        def randy():
            return randint(-half_rows, half_rows)

        return [(x, x) for x in range(self.num_ants)]
        # return [(randx(), randy()) for _ in range(self.num_ants)]

    def make_rand_dir_list(self) -> List[Dir]:
        return [randint(0, 3) for _ in range(self.num_ants)]

    def run(self):
        done = False

        pygame.init()
        screen_size = [
            (self.cell_width + self.cell_margin) * self.cols + self.cell_margin,
            (self.cell_height + self.cell_margin) * self.rows + self.cell_margin
        ]
        screen = pygame.display.set_mode(screen_size)

        pygame.display.set_caption("Langton's Ant")
        clock = pygame.time.Clock()

        step_count = 0
        coords = self.lang_ant_sim.get_coords()
        coords.update({pos: -1 for pos in self.lang_ant_sim.cur_pos_list})
        grid_w_coords = self.update_grid_with_coords(self.grid, coords)
        self.redraw_screen(grid_w_coords, screen)
        while not done:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    done = True
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_n:
                        step_count = self.step_rate
                    if event.key == pygame.K_k:
                        scale = 2
                        self.step_rate *= scale
                        step_count *= scale
                        print("step rate: {}".format(self.step_rate))
                    if event.key == pygame.K_j:
                        scale = 2
                        self.step_rate = max(1, self.step_rate // scale)
                        if step_count:
                            step_count = max(1, step_count // scale)
                        print("step rate: {}".format(self.step_rate))
                elif event.type == pygame.KEYUP:
                    if event.key == pygame.K_n:
                        step_count = 0

            if step_count:
                self.lang_ant_sim.advance(step_count)
                coords = self.lang_ant_sim.get_coords()
                coords.update({pos: -1 for pos in self.lang_ant_sim.cur_pos_list})
                self.redraw_coords(coords, screen)

            clock.tick(self.clock_speed)
            pygame.display.flip()

        pygame.quit()

    def draw_rect(self, screen, grid_pos, val):
        color = self.color_dict.get(val, WHITE)
        pygame.draw.rect(
            screen,
            color,
            [self.cell_margin + (self.cell_width + self.cell_margin) *
             grid_pos[0],
             self.cell_margin + (self.cell_height + self.cell_margin) *
             grid_pos[1],
             self.cell_width,
             self.cell_height]
        )

    def redraw_coords(self, coords, screen):
        for pos, val in coords.items():
            grid_pos = self.pos_to_grid_pos(pos, self.grid)
            self.draw_rect(screen, grid_pos, val)

    def redraw_screen(self, grid_w_coords, screen):
        screen.fill(BLACK)
        for row in range(len(self.grid)):
            for col in range(len(self.grid[0])):
                self.draw_rect(screen, (col, row), grid_w_coords[row][col])

    @classmethod
    def update_grid_with_coords(
            cls, grid: List[List], coords: Dict[Pos, State]
    ) -> List[List]:
        for pos, value in coords.items():
            grid_pos = cls.pos_to_grid_pos(pos, grid)
            if cls.is_on_grid(grid_pos, grid):
                grid[grid_pos[1]][grid_pos[0]] = value
        return grid

    @classmethod
    def pos_to_grid_pos(cls, pos: Pos, grid: List[List]) -> Pos:
        x, y = pos
        grid_x = x + len(grid[0]) // 2
        grid_y = (-1 * y) + len(grid) // 2
        return grid_x, grid_y

    @classmethod
    def is_on_grid(cls, pos: Pos, grid: List[List]) -> bool:
        return (pos[0] >= 0 and pos[1] >= 0
                and pos[0] < len(grid[0]) and pos[1] < len(grid))
