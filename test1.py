import pygame as pg

pg.init()
pg.display.init()
window = pg.display.set_mode((1000, 1000))
pg.draw.rect(window, (255, 0, 255, 255), (500, 500, 100, 100))
pg.display.flip()
