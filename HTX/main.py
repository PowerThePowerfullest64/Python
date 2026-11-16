import time
from turtle import *

t = Turtle()
t.speed(10)
for i in range(1000):
    angle = 360 / (i/100+1)
    t.forward(100/angle)
    t.left(angle)