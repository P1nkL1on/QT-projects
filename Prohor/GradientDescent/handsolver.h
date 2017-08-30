#ifndef HANDSOLVER_H
#define HANDSOLVER_H
#include "manipulator.h"
#include "QVector2D"
#include "Linegraphics/linegraphics.h"
#include "Derivable/derivable.h"

class HandSolver
{
private:
    Manipulator* maniOriginal;
    Manipulator maniCurrent;
    LineGraphics lg0;
    QVector<float> currentStep;
    QVector2D pointFinal;
public:

    void Restart(QVector2D newPoint);
    HandSolver();
    HandSolver(Manipulator* original, QVector2D final);
    void DrawItSelf (QPainter* qp, int wid, int hei) const;
    void step();

    Manipulator rotateOriginal (QVector<float> step);
    float stepMult = .0001;
    bool stop = false;

    template<typename T>
    static T CurDist ( QVector<QPair<T,T>> distsAndAngles,  QVector2D finalPoint,
                       QVector<T> transformVector){
        if (distsAndAngles.length() <= 1)
            return T(0);
        Q_ASSERT(distsAndAngles.length() > 1 && transformVector.length() == distsAndAngles.length() - 1);
        T resX = distsAndAngles[0].first,   // x root
          resY = distsAndAngles[1].second,  // y root
          nowAngle = T(0),
          finX = T(finalPoint.x()),
          finY = T(finalPoint.y());

        for (int i = 1; i < distsAndAngles.length(); i++){
            nowAngle = nowAngle + distsAndAngles[i].second + transformVector[i-1]; // summ angle
            resX = resX + Derivable::Dcos ( nowAngle ) * distsAndAngles[i].first;
            resY = resY + Derivable::Dsin ( nowAngle ) * distsAndAngles[i].first;
        }
        return Derivable::Dpow(resX - finX,2) + Derivable::Dpow(resY - finY, 2);
    }
};

#endif // HANDSOLVER_H
