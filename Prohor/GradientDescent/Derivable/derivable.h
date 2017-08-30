#ifndef DERIVABLE_H
#define DERIVABLE_H
#include "qmath.h"
#include "QVector3D"
#include "qvector.h"

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
    static float Dcos(float f) {
        return cos(f);
    }
    static float Dsin(float f) {
        return sin(f);
    }
    static float Dpow (float f, unsigned int power){
        return pow(f,power);
    }
    static QVector<Derivable> AutoDiff(QVector<QPair<Derivable, Derivable> > modelOriginal,
                                       QVector<QPair<Derivable, Derivable> > modelFinal, QVector3D transform);
    template<typename T>
    static T CurDist ( QVector<QPair<T,T>> originalPoints,  QVector<QPair<T,T>> finalPoints, QVector<T> transformVector){
        Q_ASSERT (transformVector.length() == 3);
        T res = T(0);
        for (int i = 0; i < originalPoints.length(); i++){
            T xO = originalPoints[i].first,
              yO = originalPoints[i].second,
              xF = finalPoints[i].first,
              yF = finalPoints[i].second;
            res = res
                     +Derivable::Dpow(xO * Derivable::Dcos(transformVector[2]) - yO * Derivable::Dsin(transformVector[2]) + transformVector[0] - xF,2)
                     +Derivable::Dpow(xO * Derivable::Dsin(transformVector[2]) + yO * Derivable::Dcos(transformVector[2]) + transformVector[1] - yF,2);
        }
        return res;
    }
};




#endif // DERIVABLE_H
