#ifndef DERIVABLE_H
#define DERIVABLE_H
#include "qmath.h"
#include "QVector3D"
#include "qvector.h"

namespace DerOperations {

class Derivable
{
    double value, pr_value;
public:
    Derivable():value(0), pr_value(0){}
    Derivable(float val) : value(val), pr_value(0){}
    Derivable(double val) : value(val), pr_value(0){}
    Derivable(double val, double pr_val) : value(val), pr_value(pr_val){}

    static Derivable Variable(double X){return Derivable(X, 1);}

    double getValue () const {return value; }
    double getProiz () const {return pr_value; }

    void setPrValue (double val){
        pr_value = val;
    }

    Derivable operator+ (const Derivable a) const{
        return Derivable(value + a.value, pr_value + a.pr_value);
    }
    Derivable operator- (const Derivable a) const{
        return Derivable(value - a.value, pr_value - a.pr_value);
    }
    Derivable operator* (const Derivable a) const{
        return Derivable(value * a.value, a.value * pr_value + value * a.pr_value);
    }
    Derivable operator/ (const Derivable a) const{
        return Derivable(value / a.value, (pr_value * a.value + a.pr_value * value)/ (a.value * a.value));
    }

    static Derivable cos(const Derivable f) {
        return Derivable(cos(f.value), -sin(f.value)*f.pr_value);
    }
    static Derivable sin(const Derivable f) {
        return Derivable(sin(f.value), cos(f.value)*f.pr_value);
    }
    static Derivable pow (const Derivable f, const unsigned int power){
        if (power == 1) return f;
        else return Derivable( pow(f.value,power), power * f.pr_value * pow(f.value, power - 1) );
    }
    static double cos(const double f) {
        return cos(f);
    }
    static double sin(const double f) {
        return sin(f);
    }
    static double pow (const double f,const unsigned int power){
        return pow(f,power);
    }

    static QVector<Derivable> AutoDiff(QVector<QPair<Derivable, Derivable> > modelOriginal,
                                       QVector<QPair<Derivable, Derivable> > modelFinal, QVector3D transform);
    template<typename T>
    static T CurDist ( QVector<QPair<T,T>> originalPoints,  QVector<QPair<T,T>> finalPoints, QVector<T> transformVector){
        Q_ASSERT (transformVector.length() == 3);
        T res = T(0.0);
        for (int i = 0; i < originalPoints.length(); i++){
            T xO = originalPoints[i].first,
              yO = originalPoints[i].second,
              xF = finalPoints[i].first,
              yF = finalPoints[i].second;
            res = res
                     +Derivable::pow(xO * Derivable::cos(transformVector[2]) - yO * Derivable::sin(transformVector[2]) + transformVector[0] - xF,2)
                     +Derivable::pow(xO * Derivable::sin(transformVector[2]) + yO * Derivable::cos(transformVector[2]) + transformVector[1] - yF,2);
        }
        return res;
    }
};

}


#endif // DERIVABLE_H
