import time

g = 9.82

print(" ========== TYNGDEKAFT UDREGNER ========== ")

time.sleep(1)
print("\n")

mass = 0
while True:
    try:
        mass = float(input("Indtast massen på objektet du vil måle tyngdekraften på i kg. "))
        break
    except:
        print("Massen af genstanden du indtastede må kun bestå af numre.\n\n")

fg = mass * g
print(f"Tyngdekraften af et objekt med en masse på {mass} kg er {fg}N!")
