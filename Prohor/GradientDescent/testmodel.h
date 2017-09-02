#ifndef TESTMODEL_H
#define TESTMODEL_H

#include "qvector.h"
#include "qvector2d.h"
#include "qpainter.h"
#include "derivable.h"

class TestModel
{
public:
    QVector<QVector2D> vertexes;
    TestModel();
    TestModel (QVector<QVector2D> verts);
    virtual void drawItself (QPainter* qp, const int wid, const int hei, const QColor clr) const;
    virtual void drawFromItTo (QPainter* qp, const int wid, const int hei, const TestModel* another) const;
    virtual QVector<QPair<double,double>> todoubleVector () const;
    virtual QVector<QPair<Derivable,Derivable>> toDerivableVector () const;
};

#endif // TESTMODEL_H
