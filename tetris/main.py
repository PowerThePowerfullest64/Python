import time
import msvcrt

import surface
from render import render
from rotate import rotate

import os

os.system("")

def set_tps(target):
    global tps, tick_dur
    tps = target
    tick_dur = 1 / target

def easeOutQuad(x) -> float:
    return 1 - (1 - x) * (1 - x)

def calc_tps():
    global start_time
    elapsed = time.time() - start_time
    
    return max(easeOutQuad(elapsed/3000)*20, 1.0)

previous_time = time.perf_counter()

key_mapping = {
    'H': "Up",
    'P': "Down",
    'K': "Left",
    'M': "Right",
}

def get_input():
    # If no key is in the buffer, exit immediately without blocking
    if not msvcrt.kbhit():
        return None

    raw_key = msvcrt.getch()
    key_name = None

    # Handle prefix bytes for arrow keys and special keys
    if raw_key in (b'\xe0', b'\x00'):
        special_key = msvcrt.getch().decode('utf-8', errors='ignore')

        
        key_name = key_mapping.get(special_key, f"Special_{special_key}")
    else:
        key_name = raw_key.decode('utf-8', errors='ignore')

    return key_name

def update_input() -> bool:
    moved = False
    while msvcrt.kbhit():
        key = get_input()
        if key == "Left":
            sf.move(-1, 0)
            moved = True
        elif key == "Right":
            sf.move(1, 0)
            moved = True
        elif key == "Down":
            sf.move(0, 1)
            moved = True
        elif key == "Up":
            sf.piece = rotate(sf.piece, sf)
            moved = True
        elif key == ' ':
            sf.drop()
            moved = True
        elif key.lower() == 'q':
            sf.game_over()

    return moved

if __name__ == "__main__":
    sf = surface.Surface((10, 20))
    print("\n" * (sf.height + 10), end="")  # move down so we don't delete old terminal

    acc = 0.0
    tps = 1.0
    tick_dur = 1.0 / tps

    start_time = time.time()

    while True:
        now = time.perf_counter()
        dt = now - previous_time
        previous_time = now
        acc += dt
        
        moved = update_input()

        ticked = False
        while acc >= tick_dur:
            acc -= tick_dur
            sf.update()
            ticked = True
        
        if moved or ticked:
            render(sf)
            print(f"Speed: {round(calc_tps(), 2)}", end=" ")
            print(f"Score: {sf.score}", end=" ")

        set_tps(min(20, calc_tps())) #calc is short for calculator

        time.sleep(1/60)