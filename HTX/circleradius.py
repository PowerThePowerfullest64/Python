import math
import time

def omkreds(r):
    return r * 2 * math.pi
def areal(r):
    return r * r * math.pi

navn = input("Navn = ")
print("\n")

time.sleep(1)
print("Parallelizing work...")
time.sleep(3)
print("Training neural networks...")
time.sleep(4)

print("\n")

radius = 0
try:
    radius = float(input("Radius = "))
except:
    time.sleep(4)
    print("Bro, only input numbers!")
    exit()

print("\n")

time.sleep(2)
print("Thinking...")
time.sleep(3)
print("Thinking harder")
time.sleep(4)
print("\n")

print(f"Omkreds: {omkreds(radius)}")

print("\n")

print("Contemplating...")
time.sleep(1)
print("Calculating...")
time.sleep(3)

print("\n")

print(f"Areal: {areal(radius)}")

print("\n")

time.sleep(2)
print("Doing final triangulations...")
time.sleep(3)
print("Calculating matrices...")
time.sleep(2)

print("\n")

print(f"Farvel, {navn}!")

print("\n")

time.sleep(2)
print("Caching data...")
time.sleep(2)
print("Saving user information...")
time.sleep(3)
print("Closing stuff...")
time.sleep(2)
