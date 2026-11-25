def plus():
    print("Du har valgt plus!")
    a = float(input("Indtast første tal: "))
    b = float(input("Indtast andet tal: "))
    print(f"Addition: {a} + {b} = {a+b}")

def minus():
    print("Du har valgt minus!")
    a = float(input("Indtast første tal: "))
    b = float(input("Indtast andet tal: "))
    print(f"Subtraktion: {a} - {b} = {a-b}")

def gange():
    print("Du har valgt gange!")
    a = float(input("Indtast første tal: "))
    b = float(input("Indtast andet tal: "))
    print(f"Subtraktion: {a} * {b} = {a*b}")

def division():
    print("Du har valgt gange!")
    a = float(input("Indtast første tal: "))
    b = float(input("Indtast andet tal: "))
    print(f"Subtraktion: {a} / {b} = {a/b}")

def menu():
    print() #Printer en tom linie
    print("-----")
    print("Menu:")
    print("-----")

    print("1: Addition (+)")
    print("2: Subtraktion (-)")
    print("3: Multiplikation (*)")
    print("4: Division (/)")
    print("")
    print("9: afslut")
    valg = input("Indtast dit valg: ")
    print()

    if valg == "1":
        plus()
        menu()
    elif valg == "2":
        minus()
        menu()
    elif valg == "3":
        gange()
        menu()
    elif valg == "4":
        division()
        menu()
    elif valg == "9":
        print("Afslutter! ses!")
        #Her kalder vi ikke menu, så programmet slutter.
    else:
        print("     Forkert valg... prøv igen")
        menu()



#Alt ovenstående kode er defineret i funktioner, så det her er faktisk det eneste der bliver kørt:
print("------------------------------------")
print("   Velkommen til regneprogrammet!")
print("------------------------------------")
menu()