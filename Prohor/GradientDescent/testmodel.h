#ifndef TESTMODEL_H
#define TESTMODEL_H

#include "qvector.h"
#include "qvector2d.h"
#include "qpainter.h"

class TestModel
{
private:
    QVector<QVector2D> vertexes;
public:
    TestModel();
    TestModel (QVector<QVector2D> verts);
    unsigned int vertexCount() const;
    QVector2D GetVertex (unsigned int index) const;
    void drawItself (QPainter* qp, int wid, int hei, QColor clr) const;
};

#endif // TESTMODEL_H
