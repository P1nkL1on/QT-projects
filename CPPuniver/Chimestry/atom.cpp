#include "atom.h"



Atom::Atom()
{
    name = "--";
    mass = 0;
    levelCount = 1;
    level = new int[1]{1};
}

Atom::Atom(char *atomName, int atomMass, int atomLevelCount, int* atomLevel)
{
    name = atomName;
    mass = atomMass;
    levelCount = atomLevelCount;
    level = atomLevel;
}


void Atom::Trace() const
{
    int elc = getElectronCount(), lc = -1;
    cout << name  << "  ( #" << elc << " )"<< endl << "  Mass: " << mass <<  endl << "  e- : ";
    while (levelCount - (lc++) - 1) cout << " " << level[lc];
    cout << "  @" << getValentElectronCount() <<  endl << "  p+ :  " << getProtonCount() << endl << "  n0 :  " << getNeironCount() << endl;
}


Atom::Atom(int atomNumber)
{
    switch (atomNumber) {
    case 1:
        *this = Atom("H ", 1, 1, new int[1]{1});
        break;
    case 2:
        *this = Atom("He", 4, 1, new int[1]{2});
        break;
    case 3:
        *this = Atom("Li", 7, 2, new int[2]{2,1});
        break;
    case 4:
        *this = Atom("Be", 9, 2, new int[2]{2,2});
        break;
    case 5:
        *this = Atom("B ", 11, 2, new int[2]{2,3});
        break;
    case 6:
        *this = Atom("C ", 12, 2, new int[2]{2,4});
        break;
    case 7:
        *this = Atom("N ", 14, 2, new int[2]{2,5});
        break;
    case 8:
        *this = Atom("O ", 16, 2, new int[2]{2,6});
        break;
    case 9:
        *this = Atom("F ", 19, 2, new int[2]{2,7});
        break;
    case 10:
        *this = Atom("Ne", 20, 2, new int[2]{2,8});
        break;
    case 11:
        *this = Atom("Na", 23, 3, new int[3]{2,8,1});
        break;
    case 12:
        *this = Atom("Mg", 24, 3, new int[3]{2,8,2});
        break;
    case 13:
        *this = Atom("Al", 26, 3, new int[3]{2,8,3});
        break;
    case 14:
        *this = Atom("Si", 28, 3, new int[3]{2,8,4});
        break;
    case 15:
        *this = Atom("P", 31, 3, new int[3]{2,8,5});
        break;
    case 16:
        *this = Atom("S ", 32, 3, new int[3]{2,8,6});
        break;
    case 17:
        *this = Atom("Cl", 35, 3, new int[3]{2,8,7});
        break;
    case 18:
        *this = Atom("Ar", 40, 3, new int[3]{2,8,8});
        break;
//    case 26:
//        *this = Atom("Fe", 56, 4, new int[4]{2,8,14,2});
//        break;
    default:
        break;
    }
}

Atom::Atom(char *atomNameStr)
{
    int ind = -1;
    for (int i = 0; i < 18; i++)
        if (atomNameStr[0] == atomnames[i][0] && atomNameStr[1] == atomnames[i][1])
            ind = i + 1;
    *this = Atom(ind);
}


int Atom::getElectronCount() const
{
    int lc = levelCount, summ = 0;
    while (lc--) summ += level[lc];
    return summ;
}

int Atom::getProtonCount() const
{
    return getElectronCount();
}

int Atom::getNuclonCount() const
{
    return mass;
}

int Atom::getNeironCount() const
{
    return getNuclonCount() - getProtonCount();
}

int Atom::getMass() const
{
    return mass;
}

char *Atom::getName() const
{
    return name;
}



int Atom::getValentElectronCount() const
{
    int maxE = 2, l = 1;
    while (maxE < level[levelCount - 1]){
        maxE = 2 * l * l;
        l++;
    }
    return (maxE - level[levelCount - 1]) * getValentSide();
}

int Atom::getValentSide() const
{
    int number = getElectronCount(), column =
            (number > 2)? ((number - 3) % 8 + 1) : (1 + 7 * (number == 2));
    switch (column) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            return 1;
        case 6:
        case 7:
            return -1;
        default:
            return 0;
    }
}
