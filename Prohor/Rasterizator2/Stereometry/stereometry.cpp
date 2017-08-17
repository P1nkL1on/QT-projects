#include "stereometry.h"
#include "iostream"

using namespace std;

    void Stereometry::Normalize (const QVector3D constBegin, QVector3D& movingEnd){
        movingEnd = Summ (constBegin, Mult (Resid(movingEnd, constBegin),
                                            1.0 / Dist(constBegin, movingEnd)));
        // mE = cB + (mE - cB) / long
    }

    double Stereometry::Dist (const QVector3D vert1, const QVector3D vert2){
        return sqrt( pow(vert1.x() - vert2.x(), 2)+ pow( vert1.y() - vert2.y(), 2)+ pow (vert1.z() - vert2.z(), 2) );
    }

    double Stereometry::Angle (const QVector3D vertLeft, QVector3D vertCenter, QVector3D vertRight){

        if (vertLeft == vertCenter || vertCenter == vertRight || vertRight == vertLeft) return 0.0;

        return acos(Dot( Resid (vertCenter, vertLeft), Resid (vertRight, vertCenter) )
                    /(Dist(vertLeft, vertCenter) * Dist(vertRight, vertCenter)));
        // (a * b) / (|a| * |b|)
    }

    QVector3D Stereometry::Cross (const QVector3D a, const QVector3D b){
        return QVector3D( (a.y() * b.z() - a.z() * b.y()),
                         -(a.x() * b.z() - a.z() * b.x()),
                          (a.x() * b.y() - a.y() * b.x()));
    }

    double Stereometry::Dot (const QVector3D vector1, const QVector3D vector2){
        return vector1.x() * vector2.x() + vector1.y() * vector2.y() + vector1.z() * vector2.z();
    }

    QVector3D Stereometry::Summ (const QVector3D vector1, const QVector3D vector2){
        return QVector3D( vector1.x() + vector2.x(), vector1.y() + vector2.y(), vector1.z() + vector2.z() );
    }

    QVector3D Stereometry::Resid (const QVector3D vector1, const QVector3D vector2){
        return QVector3D( vector1.x() - vector2.x(), vector1.y() - vector2.y(), vector1.z() - vector2.z() );
    }

    QVector3D Stereometry::Mult (const QVector3D vector, const double Koef){
        return QVector3D(vector.x() / Koef, vector.y() / Koef, vector.z() / Koef);
    }   

    double Stereometry::Length(const QVector3D vert)
    {
        return sqrt( pow(vert.x(), 2)+ pow( vert.y(), 2)+ pow (vert.z(), 2) );
    }
