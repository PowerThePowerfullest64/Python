import numpy as np
import time
from numba import njit
import pygame as pg

class Plane:
    def __init__(self):
        self.width, self.height = 32, 32
        self.square_size = 32
        self.bombs: list[int] = [0] * (self.width * self.height)
        self.revealed: list[bool] = [False] * (self.width * self.height)
        self.marked: list[int] = []
        self.revealed_color = (200, 200, 200)
        self.hidden_color = (120, 120, 120)
        self.marked_color = (100, 100, 150)
        self.grid_color = "blue"

    def inside(self, index: int):
        return index >= 0 and index < self.width * self.height

    def draw(self, window: pg.Surface):
        for y in range(self.height):
            for x in range(self.width):
                rect: pg.rect = (x * self.square_size, y * self.square_size, self.square_size, self.square_size)
                color: any
                index: int = y * self.width + x
                if index in self.marked:
                    color = self.marked_color
                elif self.revealed[index]:
                    color = self.revealed_color
                else: color = self.hidden_color
                pg.draw.rect(window, color, rect)

        # draws lines for grid
        for x in range(self.width):
            start = (x * self.square_size, 0)
            end = (x * self.square_size, self.height * self.square_size)
            pg.draw.line(window, self.grid_color, start, end)
        for y in range(self.height):
            start = (0, y * self.square_size)
            end = (self.width * self.square_size, y * self.square_size)
            pg.draw.line(window, self.grid_color, start, end)
    
    def reveal(self, x: int, y: int):
        index: int = y * self.width + x

        if not self.inside(index):
            return
        if self.revealed[index]:
            return
        if index in self.marked:
            return # cannot reveal marked cell!

        self.revealed[index] = True
        print(f"Revealed square ({x}, {y})!")

    def mark(self, x: int, y: int):
        index: int = y * self.width + x

        if not self.inside(index):
            return
        if self.revealed[index]:
            return

        if index in self.marked:
            self.marked.remove(index)
            return

        self.marked.append(index)
        
        print(f"Marked square ({x}, {y})!")

pg.init()
window = pg.display.set_mode((1024, 1024))
pg.display.set_caption("Minesweeper")
clock = pg.time.Clock()
running = True
plane: Plane = Plane()

def catch_events() -> tuple[bool, bool]:
    global running
    left_mouse_up = False
    right_mouse_up = False
    for event in pg.event.get():
        if event.type == pg.QUIT:
            running = False
        if event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE:
            running = False
        if event.type == pg.MOUSEBUTTONUP:
            if event.button == 1:
                left_mouse_up = True
            elif event.button == 3:
                right_mouse_up = True

    return left_mouse_up, right_mouse_up

while running:
    left_mouse_up, right_mouse_up = catch_events()

    if left_mouse_up or right_mouse_up:
        x, y = pg.mouse.get_pos()
        x //= plane.square_size
        y //= plane.square_size

        if left_mouse_up:
            plane.reveal(x, y)
        if right_mouse_up:
            plane.mark(x, y)

    window.fill("blue")

    plane.draw(window)

    pg.display.flip()

    clock.tick(60)

pg.quit()