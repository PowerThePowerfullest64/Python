import random
from turtle import *

def draw_point(t: Turtle, x: int, y: int):
    t.penup()
    t.goto((x, y))
    t.pendown()
    t.circle(1)
    t.penup()

t: Turtle = Turtle()
t.speed(0)

xCoords = []
yCoords = []


# 3 random points
for i in range(3):
    xCoords.append(random.randint(-500, 500))
    yCoords.append(random.randint(-500, 500))

    draw_point(t, xCoords[i], yCoords[i])

t.penup()

rand_index = random.randint(0, 2)
start_x = xCoords[rand_index]
start_y = yCoords[rand_index]

i = 0
while (i < 1000000):
    i += 1
    dest_index = random.randint(0, 2)
    dest_x = xCoords[dest_index]
    dest_y = yCoords[dest_index]

    start_x = (start_x + dest_x) / 2
    start_y = (start_y + dest_y) / 2

    draw_point(t, start_x, start_y)






start_x = 10