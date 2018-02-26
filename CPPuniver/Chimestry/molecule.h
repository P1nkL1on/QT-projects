#ifndef MOLECULE_H
#define MOLECULE_H

#include "atom.h"

class Molecule
{
private:
    char* name;
    int atomCount;
    Atom* atoms;
public:
    Molecule();
    Molecule (char *molName, int molAtomCount, Atom *molAtoms);
    Molecule (const int randomSize);

    void Trace() const;

    int CountAtomType (int type) const;
    int CountAtomType (char* type) const;

    int getElectronCount () const;
    int getProtonCount () const;
    int getNuclonCount () const;
    int getNeironCount () const;
    int getMass () const;
};

#endif // MOLECULE_H
