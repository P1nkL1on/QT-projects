#ifndef HANDSOLVER3D_H
#define HANDSOLVER3D_H

#include "manipulator3d.h"
#include "derivable.h"
#include "Eigen/Core"

using namespace DerOperations;
using namespace Eigen;

class HandSolver3D : public GraphicsObjectStruct::GraphicsObject
{
private:
    Manipulator3D* original;
    Manipulator3D* current;
    QVector3D finalPoint;
public:
    HandSolver3D();
    HandSolver3D(Manipulator3D* m, QVector3D fp);
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
    template <typename T>
    static T CurrentDist ( const QVector3D targetPoint, const QVector3D startPoint,
                           const QVector<T> dists, const QVector<T> anglesx,
                           const QVector<T> anglesy, const QVector<T> anglesz ){
        //QVector<T> anglesx,  anglesy, anglesz, dists;
        //original->GetDistsAndAngles(dists,anglesx, anglesy, anglesz);
        // now we have derivable/float/dist
        Matrix<double,3,1> original(3,1);
        original(0,0) = T(startPoint.x());
        original(1,0) = T(startPoint.y());
        original(2,0) = T(startPoint.z());

        for (int i = 0; i < dists.length(); i++){
            Matrix<double, 3, 3> rotat(3,3);
            rotat(0,0) = cos(anglesx[i])*cos(anglesz[i]) - sin(anglesx[i])*cos(anglesy[i])*sin(anglesz[i]);
            rotat(0,1) = -cos(anglesx[i])*sin(anglesz[i]) - sin(anglesx[i])*cos(anglesy[i])*cos(anglesz[i]);
            rotat(0,2) = sin(anglesx[i])*sin(anglesy[i]);
            rotat(1,0) = sin(anglesx[i])*cos(anglesz[i]) + cos(anglesx[i])*cos(anglesy[i])*sin(anglesz[i]);
            rotat(1,1) = -sin(anglesx[i])*sin(anglesz[i]) + cos(anglesx[i])*cos(anglesy[i])*cos(anglesz[i]);
            rotat(1,2) = -cos(anglesx[i])*sin(anglesy[i]);
            rotat(2,0) = sin(anglesy[i]) * sin(anglesz[i]);
            rotat(2,1) = sin(anglesy[i])*cos(anglesz[i]);
            rotat(2,2) = cos(anglesy[i]);
            Matrix<double, 3, 1> leng(3,1);
            leng(0,0) = dists[i]; leng(1,0) = 0; leng(2,0) = 0;

            original = rotat * leng + original;
        }
        //____

        T sx = original(0,0), sy = original(1,0), sz = original(2,0),
          fx = T(targetPoint.x()), fy = T(targetPoint.y()), fz = T(targetPoint.z());
        // total distance
        return pow(fx - sx, 2) + pow(fy - sy, 2) + pow(fz - sz, 2);
    }
};

#endif // HANDSOLVER3D_H
