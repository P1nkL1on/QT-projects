#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include "testmodel.h"

class Manipulator : public TestModel
{
public:
    QVector<double> angles;
    QVector<double> dists;
    QVector2D startPoint;

    Manipulator();
    Manipulator(QVector2D stPt, QVector<double> ds);
    Manipulator(QVector2D stPt, QVector<double> ds, QVector<double> angs);

    void drawItself (QPainter* qp, const int wid, const int hei, const QColor clr) const override;
    void drawFromItTo (QPainter* qp, const int wid, const int hei, const TestModel* another) const override;

    QVector<QPair<double,double>> todoubleVector () const override;
    QVector<QPair<Derivable,Derivable>> toDerivableVector () const override;
};

#endif // MANIPULATOR_H
