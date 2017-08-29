#ifndef MODEL_H
#define MODEL_H

#include "qvector.h"
#include "qvector2d.h"
#include "qpainter.h"

class Model
{
public:
    QVector<QVector2D> vertexes;
    TestModel();
    TestModel (QVector<QVector2D> verts);
    void drawItself (QPainter* qp, const int wid, const int hei, const QColor clr) const;
    void drawFromItTo (QPainter* qp, const int wid, const int hei, const Model* another) const;
};

#endif // TESTMODEL_H
