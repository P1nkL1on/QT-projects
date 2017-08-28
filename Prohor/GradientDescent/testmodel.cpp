#include "testmodel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(QVector<QVector2D> verts)
{
    vertexes = verts;
}

unsigned int TestModel::vertexCount() const
{
    return vertexes.length();
}

QVector2D TestModel::GetVertex(unsigned int index) const
{
    Q_ASSERT(vertexes.length() > 0 && index >=0 && vertexes.length() > index);
    return vertexes[index];
}

void TestModel::drawItself(QPainter *qp, int wid, int hei, QColor clr) const
{
    QPen pen;
    pen.setWidth(5);
    pen.setColor(clr);
    qp->setPen(pen);
    for (int i = 0; i < vertexes.length(); i++)
        qp->drawPoint(vertexes[i].x() * 4 + wid / 2, -vertexes[i].y() * 4 + hei / 2);
}
