#ifndef CHIMSTRUCT_H
#define CHIMSTRUCT_H

#include "cstdlib"
#include "iostream"
#include <stdio.h>
using namespace std;

static char atomnames[18][3] = {"H ", "He","Li","Be","B ","C ","N ","O ","F ","Ne","Na","Mg","Al","Si","P ","S ","Cl","Ar"};
struct Atom {
private:
    char *name;
    int mass, levelCount, *level;
    int getValentSide ()  const;
public:
    Atom ();
    Atom (char *atomName, int atomMass, int atomLevelCount, int *atomLevel);
    Atom (int atomName);
    Atom (char* atomNameStr);

    int getElectronCount () const;
    int getProtonCount () const;
    int getNuclonCount () const;
    int getNeironCount () const;
    int getMass () const;
    char* getName() const;
    int getValentElectronCount () const;

    void Trace () const;
};





#endif // CHIMSTRUCT_H
