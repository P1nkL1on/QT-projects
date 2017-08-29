#include "testmodel.h"

TestModel::TestModel()
{

}

TestModel::TestModel(QVector<QVector2D> verts)
{
    vertexes = verts;
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
    if (vertexes.length() != another->vertexes.length() )
        return;
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);
    qp->setPen(pen);
    for (int i = 0; i < vertexes.length(); i++)
        qp->drawLine((int)(vertexes[i].x() * 4 + wid / 2), (int)(-(vertexes[i].y()) * 4 + hei / 2),
                     (int)(another->vertexes[i].x() * 4 + wid / 2), (int)(-(another->vertexes[i].y()) * 4 + hei / 2));
}

QVector<QPair<float, float> > TestModel::toFloatVector() const
{
    QVector<QPair<float, float> > res;
    for (int i = 0; i < vertexes.length(); i++){
        res << QPair<float, float>(vertexes[i].x(), vertexes[i].y());
    }
    return res;
}

QVector<QPair<Derivable, Derivable> > TestModel::toDerivableVector() const
{
    QVector<QPair<Derivable, Derivable> > res;
    for (int i = 0; i < vertexes.length(); i++){
        res << QPair<Derivable, Derivable>(Derivable(vertexes[i].x()), Derivable(vertexes[i].y()));
    }
    return res;
}
