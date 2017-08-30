#include "manipulator.h"
#include "qmath.h"
#include "QPen"

Manipulator::Manipulator()
{

}

Manipulator::Manipulator(QVector2D stPt, QVector<float> ds)
{
    startPoint = stPt;
    dists = ds; angles;
    for (int i = 0; i < ds.length(); i++)
        angles << 0;
}

Manipulator::Manipulator(QVector2D stPt, QVector<float> ds, QVector<float> angs)
{
    startPoint = stPt;
    dists = ds; angles;
    angles = angs;
}

void Manipulator::drawItself(QPainter *qp, const int wid, const int hei, const QColor clr) const
{
    QVector2D wasPlace = startPoint;
    float currentAngle = 0;
    for (int i = 0; i < dists.length(); i++){
        currentAngle += angles[i];
        QVector2D newPlace = QVector2D( wasPlace.x() + cos(currentAngle) * dists[i],
                                        wasPlace.y() + sin(currentAngle) * dists[i]);
        QPen pp(clr);
        pp.setWidth(1);
        qp->setPen(pp);
        qp->drawLine(wid/2 + 4 * wasPlace.x(), hei / 2 - 4 * wasPlace.y(),
                     wid/2 + 4 * newPlace.x(), hei / 2 - 4 * newPlace.y());

        pp.setWidth(3);
        qp->setPen(pp);
        qp->drawPoint(wid/2 + 4 * newPlace.x(), hei / 2 - 4 * newPlace.y());

        wasPlace = QVector2D(newPlace.x(), newPlace.y());
    }
}

void Manipulator::drawFromItTo(QPainter *qp, const int wid, const int hei, const TestModel *another) const
{
    return;
}

QVector<QPair<float, float> > Manipulator::toFloatVector() const
{
    QVector<QPair<float, float> > res;
    res << QPair<float,float>(startPoint.x(), startPoint.y());
    for (int i = 0; i < dists.length(); i++){
        res << QPair<float, float>(dists[i], angles[i]);
    }
    return res;
}

QVector<QPair<Derivable, Derivable> > Manipulator::toDerivableVector() const
{
    QVector<QPair<Derivable, Derivable> > res;
    res << QPair<Derivable,Derivable>(Derivable(startPoint.x()), Derivable(startPoint.y()));
    for (int i = 0; i < dists.length(); i++){
        res << QPair<Derivable, Derivable>(Derivable(dists[i]), Derivable(angles[i]));
    }
    return res;
}
