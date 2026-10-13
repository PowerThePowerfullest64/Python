import random
import numpy as np
from copy import deepcopy

from piece_loader import load_pieces
colors = ['red', 'magenta', 'green', 'yellow', 'blue']

class Surface:
    def __init__(self, size: tuple[int, int]):
        self.width, self.height = size
        self.cells: np.ndarray = np.zeros((self.width * self.height), dtype=int) # 0 is None, 1 is static
        self.cell_colors: np.ndarray = np.full(self.width * self.height, "white", dtype=object)

        self.pieces: np.ndarray
        self.piece: np.ndarray
        self.piece_color: str

        load_pieces(self)
        self.piece = self.pieces[random.randint(0, len(self.pieces)-1)]
        self.piece_color = colors[random.randint(0, len(colors) - 1)]

        self.score = 0
        

    def get(self, x: int, y: int) -> int:
        index = y * self.width + x
        return self.cells[index]
    
    def change(self, x: int, y: int, state: int):
        index = y * self.width + x
        self.cells[index] = state

    def inside(self, x: int, y: int) -> bool:
        return x >= 0 and x < self.width and y >= 0 and y < self.height

    def update(self):
        if self.can_move(0, 1, self.piece):
            self.piece = self.piece + self.width
        else:
            for i in self.piece:
                self.cells[i] = 1
                self.cell_colors[i] = self.piece_color
            self.check_lines()
            self.spawn_piece()
            

    def move(self, mx: int, my: int, piece = None) -> bool:
        if piece is None:
            piece = self.piece

        if not self.can_move(mx, my, piece):
            return False

        piece += mx
        piece += self.width * my

        return True

    def can_move(self, mx: int, my: int, piece = None) -> bool:
        if piece is None:
            piece = self.piece

        x = piece % self.width
        y = piece // self.width
        
        neighbor = x + mx, y + my
        
        for i in range(len(piece)):
            if not self.inside(neighbor[0][i], neighbor[1][i]):
                
                return False
            state = self.get(neighbor[0][i], neighbor[1][i])

            if state != 0:
                return False

        return True

    def spawn_piece(self):
        self.piece = self.pieces[random.randint(0, len(self.pieces)-1)]
        self.piece_color = colors[random.randint(0, len(colors)-1)]

        for i in range(len(self.cells)):
            if self.cells[i] != 1:
                continue

            for j in range(len(self.piece)):
                if i == self.piece[j]:
                    self.game_over()
                    return

    def game_over(self):
        if self.score < 1:
            print("\n\033[1m" + "\033[4m" + f"You suck\nScore: \033[32m{self.score}")
        else:    
            print("\n\033[1m" + "\033[4m" + f"!!!Game over!!! \nScore: \033[32m{self.score}")
        input("\033[0m\nPress enter to exit")
        exit()

    def drop(self):
        if len(self.piece) == 0:
            return
        while self.move(0, 1):
            pass
        self.update()



    def compute_shadow(self) -> np.ndarray:
        shadow = deepcopy(self.piece)

        while self.move(0, 1, shadow):
            pass

        return shadow        

    def check_lines(self):
        lines_cleared = 0
        y = self.height - 1

        while y >= 0:
            row_start = y * self.width
            row_end = row_start + self.width

            # Check if the entire line is filled
            if np.all(self.cells[row_start:row_end] != 0):
                lines_cleared += 1

                # Shift everything above this row down by one row
                self.cell_colors[self.width : row_end] = self.cell_colors[0 : row_start].copy() #move da colors
                self.cells[self.width : row_end] = self.cells[0 : row_start].copy() #and cells
                
                # Clear the top row
                self.cells[0 : self.width] = 0
                self.cell_colors[0 : self.width] = 0

            else:
                y -= 1

        self.score += lines_cleared**2
        return lines_cleared