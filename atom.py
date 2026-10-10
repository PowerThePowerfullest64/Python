import periodictable as pt
import chempy as cp

names = {
    0: "nothing",
    1: "hydrogen",
    2: "helium",
    3: "lithium"
}

short_names = {
    0: "nothing",
    1: "H",
    2: "He",
    3: "Li"
}

class Atom:
    def __init__(self, protons, neutrons, electrons):
        self.protons = protons
        self.neutrons = neutrons
        self.electrons = electrons

    def charge(self) -> int:
        return self.protons - self.electrons

    def name(self) -> str:
        if self.protons == 1 and self.neutrons == 0 and self.electrons == 0:
            return "proton"

        end: str = ""
        charge = self.charge()
        if charge > 0:
            end = "ion"
        elif charge < 0:
            end = "id"

        return names[self.protons] + end

    def short_name(self) -> str:
        return short_names[self.protons]

class Bond:
    def __init__(self):
        self.atoms: list[Atom]

hydrogen: Atom = Atom(3, 0, 3)
print(hydrogen.short_name())
print(cp.atomic_number("lithium"))