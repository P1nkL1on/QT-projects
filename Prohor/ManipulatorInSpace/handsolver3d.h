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
    QVector<QVector3D> currentTransofrm;
    double previousDistance;
    double step = .0001;
public:
    HandSolver3D();
    HandSolver3D(Manipulator3D* m, QVector3D fp);
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
    void Step ();
    template <typename T>
    static T CurrentDist ( const QVector3D targetPoint, const QVector3D startPoint,
                           const QVector<T> dists, const QVector<T> anglesx,
                           const QVector<T> anglesy, const QVector<T> anglesz ){
        //QVector<T> anglesx,  anglesy, anglesz, dists;
        //original->GetDistsAndAngles(dists,anglesx, anglesy, anglesz);
        // now we have derivable/float/dist
        Matrix<T,3,1> original(3,1);
        original(0,0) = T(startPoint.x());
        original(1,0) = T(startPoint.y());
        original(2,0) = T(startPoint.z());

        T   cTX = T(0.0),//anglesx[0] + T(currentTransofrm[0].x()),
            cTY = T(0.0),//anglesy[0] + T(currentTransofrm[0].y()),
            cTZ = T(0.0);//anglesz[0] + T(currentTransofrm[0].z()),

        for (int i = 0; i < dists.length(); i++){
            cTX = anglesx[i] + cTX;
            cTY = anglesy[i] + cTY;
            cTZ = anglesz[i] + cTZ;
                // forgot about increasement;
            Matrix<T, 3, 3> rotat(3,3);
            rotat(0,0) = cos(cTX)*cos(cTZ) - sin(cTX)*cos(cTY)*sin(cTZ);
            rotat(0,1) = (T(-1.0)) * cos(cTX)*sin(cTZ) - sin(cTX)*cos(cTY)*cos(cTZ);
            rotat(0,2) = sin(cTX)*sin(cTY);
            rotat(1,0) = sin(cTX)*cos(cTZ) + cos(cTX)*cos(cTY)*sin(cTZ);
            rotat(1,1) = (T(-1.0)) * sin(cTX)*sin(cTZ) + cos(cTX)*cos(cTY)*cos(cTZ);
            rotat(1,2) = (T(-1.0)) * cos(cTX)*sin(cTY);
            rotat(2,0) = sin(cTY) * sin(cTZ);
            rotat(2,1) = sin(cTY)*cos(cTZ);
            rotat(2,2) = cos(cTY);
            Matrix<T, 3, 1> leng(3,1);
            leng(0,0) = dists[i]; leng(1,0) = T(0.0); leng(2,0) = T(0.0);

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
