from turtle import *

import time

def make_polygon(t: Turtle, sides: int, sidelength: int, x: int=0, y: int=0, fill: bool=False):
    angle = 360.0 / sides
    t.penup()
    t.setpos((x, y))
    t.pendown()

    if fill:
        t.begin_fill()

    for _ in range(sides):
        t.forward(sidelength)
        t.left(angle)

    if fill:
        t.end_fill()


def make_star(t: Turtle, arms: int, arm_length: int, x: int=0, y: int=0, fill: bool=False):
    armpit_angle = 360 - (360 / arms)
    arm_angle = armpit_angle*2

    t.penup()
    t.setpos((x, y))
    t.pendown()

    if fill:
        t.begin_fill()

    for _ in range(arms):
        t.forward(arm_length)
        t.left(arm_angle)
        t.forward(arm_length)
        t.right(armpit_angle)

    if fill:
        t.end_fill()

t = Turtle()
t.speed(10)

make_star(t, arms=7, arm_length=50, fill=True)
make_polygon(t)
time.sleep(5)

