#ifndef DESCENT_H
#define DESCENT_H

#include "testmodel.h"
#include "qpainter.h"
#include "qvector3d.h"
#include "linegraphics.h"
#include "derivable.h"

class Descent
{
private:
    TestModel* modelOriginal;
    TestModel* modelFinal;
    TestModel lastApproximate;
    LineGraphics lg0;

    QVector3D currentStep;
//
//    QVector<T> CalculateQuadSumm ();
    QVector<Derivable> AutoDiff(TestModel *modelOriginal, TestModel *modelFinal, QVector3D transform);
    float CurrentDist(TestModel *originalModel, TestModel *finalModel, QVector3D derTransform);

    template<typename T>
    T CurDist ( QVector<QPair<T,T>> originalPoints,  QVector<QPair<T,T>> finalPoints, QVector<T> transformVector){
        Q_ASSERT (transformVector.length() == 3);
        T res;
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

public:
    Descent();
    Descent(TestModel* a, TestModel* b);
    void DrawItSelf (QPainter* qp, int wid, int hei) const;
    void Step();
    bool stop = false;
    static TestModel TranslateAndRotate(TestModel *originalModel, QVector3D transl);
};

#endif // DESCENT_H
