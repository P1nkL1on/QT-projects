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

void TestModel::drawItself(QPainter *qp, const int wid, const int hei, const QColor clr) const
{
    QPen pen;
    pen.setWidth(5);
    pen.setColor(clr);
    qp->setPen(pen);
    for (int i = 0; i < vertexes.length(); i++)
        qp->drawPoint((int)(vertexes[i].x() * 4 + wid / 2), (int)(-(vertexes[i].y()) * 4 + hei / 2));
}

void TestModel::drawFromItTo(QPainter *qp, const int wid, const int hei,  const TestModel *another) const
{
    if (vertexCount() != another->vertexCount() )
        return;
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);
    qp->setPen(pen);
    for (int i = 0; i < vertexes.length(); i++)
        qp->drawLine((int)(vertexes[i].x() * 4 + wid / 2), (int)(-(vertexes[i].y()) * 4 + hei / 2),
                     (int)(another->GetVertex(i).x() * 4 + wid / 2), (int)(-(another->GetVertex(i).y()) * 4 + hei / 2));
}
