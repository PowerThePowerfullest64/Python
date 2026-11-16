import random
from termcolor import colored

print("----- RNG -----")
name = input(colored("Hvad er dit navn? ", "blue"))
vip = "Anders"
while True:
    number = random.randint(1, 10)
    while True:
        try:
            guess = eval(input(colored("Gæt et helt tal mellem 1-10: ", "magenta")))
            break
        except NameError:
            pass

    if guess == number or name == vip:
        print(colored("!!!!! Det er korrekt, tallet var " + str(guess) + " !!!!!", (0, 255, 0)))
    else:
        print(colored("Det er ukorrekt og tallet er ", (255, 0, 0)) + colored(str(number), (255, 255, 0)))

    print("\n")
