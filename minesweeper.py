import numpy as np
import time
from numba import njit
import pygame as pg
import random

class Plane:
    def __init__(self):
        self.width, self.height = 32, 32
        self.state: list[int] = [0] * (self.width * self.height) # if tiles is bomb or has bombs around it
        self.revealed: list[bool] = [False] * (self.width * self.height)
        self.marked: list[int] = []
        self.hidden_color = (150, 150, 150)
        self.grid_color = (100, 100, 100)
        self.lost = False
        self.first_place = True
        self.find_empty_tries = 0
        self.max_empty_tries = 500

        self.bomb_count = 192

        self.init_bombs(self.bomb_count)
        self.set_states()

    def restart(self, soft=True):
        self.lost = False
        self.first_place = True
        self.revealed = [False] * (self.width * self.height)
        self.state = [0] * (self.width * self.height)
        self.init_bombs(self.bomb_count)
        self.set_states()

        if soft:
            self.find_empty_tries = 0
            self.marked.clear()
            print("Restarted game!")

    def init_bombs(self, count: int):
        if count >= self.width * self.height:
            print("Tried to spawn as many or more bombs than there are squares, which is not allowed.")
            return

        # try 5 times as many bombs are trying to be placed. (so 5 times per bomb average)
        for i in range(count * 5):
            index = random.randint(0, self.width * self.height-1)

            if self.state[index] < 0:
                continue

            self.state[index] = -1 # place bomb
            count -= 1

            if count <= 0:
                break

        if count > 0:
            print("Partially placed bombs because not all bombs could find a valid square.")

    def set_states(self):
        for i in range(self.width * self.height):
            self.state[i] = self.count(i)

    def inside(self, x: int, y: int):
        return x >= 0 and x < self.width and y >= 0 and y < self.height

    def draw(self, window: pg.Surface):
        for y in range(self.height):
            for x in range(self.width):
                rect: pg.rect = (x * square_size, y * square_size, square_size, square_size)
                index: int = y * self.width + x

                if not self.revealed[index]:
                    color = self.hidden_color
                    pg.draw.rect(window, color, rect)
                    if index in self.marked:
                        window.blit(flag_texture, rect)
                else:
                    state: int = self.state[index]

                    if state < 0: # bomb!
                        window.blit(bomb_texture, rect)
                        continue

                    try:
                        window.blit(revealed_textures[state], rect)
                    except IndexError:
                        print(f"IndexError: State was {state} at accessed square ({x}, {y}).")

        # draws lines for grid
        for x in range(self.width):
            start = (x * square_size, 0)
            end = (x * square_size, self.height * square_size)
            pg.draw.line(window, self.grid_color, start, end)
        for y in range(self.height):
            start = (0, y * square_size)
            end = (self.width * square_size, y * square_size)
            pg.draw.line(window, self.grid_color, start, end)
    
    def reveal(self, x: int, y: int):
        index: int = y * self.width + x

        if not self.inside(x, y):
            return
        if self.revealed[index]:
            return # already revealed!
        if index in self.marked:
            return # cannot reveal marked cell!

        if self.first_place:
            if self.state[index] != 0:
                if self.find_empty_tries >= self.max_empty_tries:
                    print("Couldn't find a valid starter position. Closing game :(")
                    self.lost = True
                    return

                self.find_empty_tries += 1
                self.restart(soft=False)
                self.reveal(x, y)
        elif self.state[index] < 0:
            self.reveal_all() # reveal all squares when mine is pressed.
            self.lost = True

        self.first_place = False
        self.revealed[index] = True

        # flood fill.
        if self.state[index] == 0:
            for rx in range(-1, 2, 1):
                for ry in range(-1, 2, 1):
                    self.reveal(x+rx, y+ry)

    def mark(self, x: int, y: int):
        index: int = y * self.width + x

        if not self.inside(x, y):
            return
        if self.revealed[index]:
            return

        if index in self.marked:
            self.marked.remove(index)
            return

        self.marked.append(index)

    def count(self, index: int) -> int:
        ax: int = index % self.width
        ay: int = index // self.width

        if not self.inside(ax, ay):
            return 0
        if self.state[index] < 0:
            return self.state[index]

        count = 0

        for rx in range(-1, 2, 1):
            for ry in range(-1, 2, 1):
                if rx == 0 and ry == 0:
                    continue

                x = ax + rx
                y = ay + ry
                i = y * self.width + x

                if not self.inside(x, y):
                    continue

                if self.state[i] < 0:
                    count += 1
        return count

    def reveal_all(self):
        for i in range(self.width * self.height):
            self.revealed[i] = True
        print("Revealed all.")


def colliding_point(point, rect) -> bool:
    return point[0] >= rect[0] and point[0] < rect[0]+rect[2] and point[1] >= rect[1] and point[1] < rect[1]+rect[3]

class Button:
    def __init__(self, x, y, w, h, color):
        self.x = x
        self.y = y
        self.w = w
        self.h = h
        self.color = color

    def is_clicked(self, lmb: bool) -> bool:
        mouse_pos = pg.mouse.get_pos()
        return colliding_point(mouse_pos, (self.x, self.y, self.w, self.h)) and lmb

    def draw(self, window: pg.Surface):
        pg.draw.rect(window, self.color, (self.x, self.y, self.w, self.h))

pg.init()
width, height = 1024, 1024
window = pg.display.set_mode((width, height))
pg.display.set_caption("Minesweeper")
clock = pg.time.Clock()
running = True
plane: Plane = Plane()

square_size = min(width, height) // max(plane.width, plane.height)
texture_path = "textures/minesweeper/"
revealed_textures = [
    pg.transform.scale(pg.image.load(texture_path + "0.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "1.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "2.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "3.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "4.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "5.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "6.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "7.png"), (square_size, square_size)),
    pg.transform.scale(pg.image.load(texture_path + "8.png"), (square_size, square_size)),
]
flag_texture = pg.transform.scale(pg.image.load(texture_path + "flag.png"), (square_size, square_size)).convert_alpha()
bomb_texture = pg.transform.scale(pg.image.load(texture_path + "bomb.png"), (square_size, square_size))
print("Loaded Textures Successfully!")

play_button: Button = Button(width/2 - 250/2, height/2 - 100/2, 250, 100, "red")
retry_button: Button = Button(width/2 - 250/2, height/2 - 100/2, 250, 100, "green")
menu_button: Button = Button(width/2 - 250/2, height/2 - 100/2 + 120, 250, 100, "blue")

game_state = "menu"

def catch_events() -> tuple[bool, bool, bool]:
    global running
    left_mouse_up = False
    right_mouse_up = False
    restart = False
    for event in pg.event.get():
        if event.type == pg.QUIT:
            running = False
        if event.type == pg.KEYDOWN and event.key == pg.K_ESCAPE:
            running = False
        if event.type == pg.KEYDOWN and event.key == pg.K_r:
            restart = True
        if event.type == pg.MOUSEBUTTONUP:
            if event.button == 1:
                left_mouse_up = True
            elif event.button == 3:
                right_mouse_up = True

    return left_mouse_up, right_mouse_up, restart

while running:
    left_mouse_up, right_mouse_up, restart = catch_events()

    if game_state == "play":
        if (left_mouse_up or right_mouse_up) and not plane.lost:
            x, y = pg.mouse.get_pos()
            x //= square_size
            y //= square_size
    
            if left_mouse_up:
                plane.reveal(x, y)
            if right_mouse_up:
                plane.mark(x, y)
            if restart:
                plane.restart()
        if plane.lost:
            game_state = "game_over"

    window.fill("blue")

    if game_state == "play" or game_state == "game_over":
        plane.draw(window)

    if game_state == "game_over":
        retry_button.draw(window)
        menu_button.draw(window)
        if retry_button.is_clicked(left_mouse_up):
            game_state = "play"
            plane.restart()
        if menu_button.is_clicked(left_mouse_up):
            game_state = "menu"
            plane.restart()

    if game_state == "menu":
        play_button.draw(window)
        if play_button.is_clicked(left_mouse_up):
            game_state = "play"
        

    pg.display.flip()

    clock.tick(60)

pg.quit()