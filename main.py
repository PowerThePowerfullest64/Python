import time
import random
import math

def sort(l: list[int]):
    for i in range(len(l)):
        swapped = False
        for j in range(len(l)-1):
            if l[j] > l[j+1]:
                swapped = True
                l[j], l[j+1] = l[j+1], l[j]
        if not swapped:
            break


my_list = []
for i in range(1000):
    my_list.append(random.randint(1, 1000))

start = time.perf_counter()

sort(my_list)

end = time.perf_counter()
duration = (end - start) * 1000

print(my_list)

print(f"Program execution took {duration} ms!")