#ifndef DERIVABLE_H
#define DERIVABLE_H
#include "qmath.h"

class Derivable
{
    float value, pr_value;
public:
    Derivable():value(0), pr_value(0){}
    Derivable(float val) : value(val), pr_value(0){}
    Derivable(float val, float pr_val) : value(val), pr_value(pr_val){}

    static Derivable Variable(float X){return Derivable(X, 1);}

    float getValue () const {return value; }
    float getProiz () const {return pr_value; }

    void setPrValue (float val){
        pr_value = val;
    }

    Derivable operator+ (Derivable a){
        return Derivable(value + a.value, pr_value + a.pr_value);
    }
    Derivable operator- (Derivable a){
        return Derivable(value - a.value, pr_value - a.pr_value);
    }
    Derivable operator* (Derivable a){
        return Derivable(value * a.value, a.value * pr_value + value * a.pr_value);
    }
    Derivable operator/ (Derivable a){
        return Derivable(value / a.value, (pr_value * a.value + a.pr_value * value)/ (a.value * a.value));
    }

    static Derivable Dcos(Derivable f) {
        return Derivable(cos(f.value), -sin(f.value)*f.pr_value);
    }
    static Derivable Dsin(Derivable f) {
        return Derivable(sin(f.value), cos(f.value)*f.pr_value);
    }

    static Derivable Dpow (Derivable f, unsigned int power){
        if (power == 1) return f;
        else return Derivable( pow(f.value,power), power * f.pr_value * pow(f.value, power - 1) );
    }
};

#endif // DERIVABLE_H
