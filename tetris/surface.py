import random
import numpy as np

class Surface:
    def __init__(self, size: tuple[int, int]):
        self.width, self.height = size
        self.cells: np.ndarray = np.zeros((self.width * self.height), dtype=int) # 0 is None, 1 is static, 2 is falling

    def get(self, x: int, y: int) -> int:
        index = y * self.width + x
        return self.cells[index]
    
    def change(self, x: int, y: int, state: int):
        index = y * self.width + x
        self.cells[index] = state

    def inside(self, x: int, y: int) -> bool:
        return x >= 0 and x < self.width and y >= 0 and y < self.height

    def update(self):
        for y in range(self.height-1, -1, -1):
            for x in range(self.width):
                if self.get(x, y) != 2:
                    continue
                if self.inside(x, y+1):
                    state = self.get(x, y+1)
                    if state == 0:
                        self.change(x, y+1, 2)
                        self.change(x, y, 0)
                    else:
                        self.cells[self.cells == 2] = 1
                        return
                else:
                    self.cells[self.cells == 2] = 1
                    return

    def move(self, mx: int):
        if not self.can_move(mx):
            return
        
        pass # move cells that are moving in the specified direction

    def can_move(self, mx: int) -> bool:
        if mx != 1 and mx != -1:
            raise ValueError("Cannot move more than 1 in either direction, therefore mx should only be 1 or -1.")

        for y in range(self.height):
            for x in range(self.width):
                if self.get(x, y) != 2:
                    continue

                if not self.inside(x + mx, y):
                    return False

                if self.get(x + mx, y) == 1:
                    return False

        return True