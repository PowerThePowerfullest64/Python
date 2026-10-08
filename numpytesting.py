import numpy as np

my_list: list[int] = [5, 1, 7, 81, 41, 2, 5, 6]
print(f"List before sort: {my_list}")

my_list = np.sort(my_list)
print(f"List after sort: {my_list}")