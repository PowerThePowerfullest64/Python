import surface as sf
from os import get_terminal_size
cell_chars: dict[int, str] = {
    0: " ",
    1: "#",
    2: "@",
}

def render(surface: sf.Surface):
    lines = get_terminal_size()[1]
    print("\n" * (lines - surface.height))
    print("-" * (surface.width * 2 + 1))
    for y in range(surface.height):
        print("|", end="")
        for x in range(surface.width):
            state = surface.get(x, y)
            print(cell_chars[state], end="")
            if x != surface.width - 1:
                print(" ", end="")

        print("|")
    print("-" * (surface.width * 2 + 1))
