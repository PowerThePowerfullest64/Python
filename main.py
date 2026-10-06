name_from_protons = {
    1: "hydrogen",
    2: "helium",
    3: "lithium"
}

class Atom:
    def __init__(self, protons, neutrons, electrons):
        self.protons = protons
        self.neutrons = neutrons
        self.electrons = electrons

    def charge(self) -> int:
        return self.protons - self.electrons

    def name(self) -> str:
        end: str = ""
        charge = self.charge()
        if charge > 0:
            end = "ium"
        elif charge < 0:
            end = "id"

        return name_from_protons[self.protons] + end

class Bond:
    def __init__(self):
        self.atoms: list[Atom]

hydrogen: Atom = Atom(1, 0, 1)
print(hydrogen.name())