#ifndef DESCENT_H
#define DESCENT_H

#include "testmodel.h"
#include "qpainter.h"
#include "qvector3d.h"
#include "Linegraphics/linegraphics.h"
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
    float CurrentDist(TestModel *originalModel, TestModel *finalModel, QVector3D derTransform);
public:
    Descent();
    Descent(TestModel* a, TestModel* b);
    void DrawItSelf (QPainter* qp, int wid, int hei) const;
    void Step();
    bool stop = false;
    static TestModel TranslateAndRotate(TestModel *originalModel, QVector3D transl);
};

#endif // DESCENT_H
