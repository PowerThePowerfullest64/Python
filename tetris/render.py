import os
import surface as sf
from termcolor import colored

cell_chars: dict[int, str] = {
    0: " ",
    1: '#',
    2: '@',
    3: "0",
}


def render(surface: sf.Surface):
    shadow = surface.compute_shadow()
    
    print("\033[H", end="") #move da cursur to start/home

    print("-" * (surface.width * 2 + 1))
    for y in range(surface.height):
        print("\033[2K", end="") #clear da current line
        print("|", end="")
        for x in range(surface.width):
            index = y * surface.width + x
            if index in surface.piece:
                state = 2
            elif index in shadow:
                state = 3
            else:
                state = surface.get(x, y)

            color = "white"

            if state == 1:
                color = surface.cell_colors[index]
            elif state == 2:
                color = surface.piece_color
            elif state == 3:
                color = "white"
            
            print(colored(cell_chars[state], color, attrs=['bold']), end="")
            if x != surface.width - 1:
                print(" ", end="")

        print("|")
    print("-" * (surface.width * 2 + 1))