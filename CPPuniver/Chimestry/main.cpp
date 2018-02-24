#include "molecule.h".h"

using namespace std;

int main(int argc, char *argv[])
{
    Molecule H2O = Molecule("water", 3, new Atom[3] {Atom("H "), Atom("O "), Atom("H ")});
    H2O.Trace();

    return 0;
}
