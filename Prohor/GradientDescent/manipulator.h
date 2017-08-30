#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include "testmodel.h"

class Manipulator : public TestModel
{
public:
    QVector<float> angles;
    QVector<float> dists;
    QVector2D startPoint;

    Manipulator();
    Manipulator(QVector2D stPt, QVector<float> ds);
    Manipulator(QVector2D stPt, QVector<float> ds, QVector<float> angs);

    void drawItself (QPainter* qp, const int wid, const int hei, const QColor clr) const override;
    void drawFromItTo (QPainter* qp, const int wid, const int hei, const TestModel* another) const override;

    QVector<QPair<float,float>> toFloatVector () const override;
    QVector<QPair<Derivable,Derivable>> toDerivableVector () const override;
};

#endif // MANIPULATOR_H
