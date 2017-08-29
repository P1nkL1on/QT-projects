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
    QVector3D stepTraRot;
    QVector3D CurrentGradientDistValue() const;
    float Module (QVector3D qv)const ;

    float Dist(QVector2D a, QVector2D b) const;
    float DistErrorFunc (TestModel* currentModel) const;
    float QuadDist(QVector2D a, QVector2D b) const;
    float DistValue (TestModel currentModel) const;

    QVector<Derivable>  AutoDiffThisShit (TestModel* originalModel, TestModel* finalModel, QVector3D transform) const;

    QVector3D RealProizv () const;
    float stepMult = .001;
public:
    Descent();
    Descent(TestModel* a, TestModel* b);

    void DrawItSelf (QPainter* qp, int wid, int hei) const;
    void Step();

    TestModel TranslateAndRotate(TestModel* originalModel, QVector3D translat) const;
    bool stop;
};

#endif // DESCENT_H
