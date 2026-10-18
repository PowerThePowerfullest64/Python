from json import load
import os
import sys
from numpy import array

def load_pieces(sf):
    if hasattr(sys, '_MEIPASS'):
        base_dir = sys._MEIPASS
    else:
        base_dir = os.path.dirname(os.path.abspath(__file__))
        
    file_path = os.path.join(base_dir, "pieces.json")
    
    # fallback to subdirectory if not found in root
    if not os.path.exists(file_path):
        file_path = os.path.join(base_dir, "tetris", "pieces.json")

    with open(file_path, "r") as file:
        data = load(file)
        pieces = []
    for piece_index in range(len(data)):
        piece = []
        for y in range(len(data[piece_index])):
            for x in range(len(data[piece_index][y])):
                state = data[piece_index][y][x]
                if state == 1:
                    index = y * sf.width + x + (sf.width // 2) - 1
                    piece.append(index)
        pieces.append(array(piece))
    sf.pieces = array(pieces)