#ifndef STEREOMETRY_H
#define STEREOMETRY_H
#include "QVector3D"

namespace Stereometry {

    void Normalize (const QVector3D constBegin, QVector3D& movingEnd);
    double Dist (const QVector3D vert1, const QVector3D vert2);
    double Length (const QVector3D vert);
    double Angle (const QVector3D vertLeft, QVector3D vertCenter, QVector3D vertRight);
    QVector3D Cross (const QVector3D vector1, const QVector3D vector2);
    double Dot (const QVector3D vector1, const QVector3D vector2);
    QVector3D Summ (const QVector3D vector1, const QVector3D vector2);
    QVector3D Resid (const QVector3D vector1, const QVector3D vector2);
    QVector3D Mult (const QVector3D vector, const double Koef);

}


#endif // STEREOMETRY_H
