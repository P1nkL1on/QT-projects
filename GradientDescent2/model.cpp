#include "model.h"

Model::Model()
{

}

Model::Model(QVector<QVector2D> verts){
    vertexes = verts;
}

void Model::drawItself(QPainter *qp, const int wid, const int hei, const QColor clr) const
{
    QPen pen;
    pen.setWidth(5);
    pen.setColor(clr);
    qp->setPen(pen);

    for (int i = 0; i < vertexes.length(); i++)
        qp->drawPoint((int)(vertexes[i].x() * 4 + wid / 2), (int)(-(vertexes[i].y()) * 4 + hei / 2));
}

void Model::drawFromItTo(QPainter *qp, const int wid, const int hei,  const Model *another) const
{
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);
    qp->setPen(pen);

    for (int i = 0; i < vertexes.length(); i++)
        qp->drawLine((int)(vertexes[i].x() * 4 + wid / 2), (int)(-(vertexes[i].y()) * 4 + hei / 2),
                     (int)(another->vertexes(i).x() * 4 + wid / 2), (int)(-(another->vertexes(i).y()) * 4 + hei / 2));
}
