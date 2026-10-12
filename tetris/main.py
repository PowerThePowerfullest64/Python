import time

import surface
from render import render

sf = surface.Surface((10, 20))
sf.change(5, 9, 2)
sf.change(5, 10, 2)
sf.change(4, 9, 2)

while True:
    sf.update()
    render(sf)
    time.sleep(1)

