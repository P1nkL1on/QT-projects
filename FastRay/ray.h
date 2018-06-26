#ifndef RAY_H
#define RAY_H

#include "QVector3D"

namespace  RayStruct {
    class Ray
    {
    private:
        QVector3D from;
        QVector3D to;
    public:
        Ray();
        Ray(QVector3D start, QVector3D finish);
        QVector3D From () const;
        QVector3D To () const;
        QVector3D ray () const;
        Ray inverse () const;
        float GetAngleBetween (const Ray& another);
    };


    class RayUtils{
    public:
        static float dot (QVector3D a, QVector3D b){
            return a.x() * b.x() + a.y() * b.y()+ a.z() * b.z();
        }

    };
}


#endif // RAY_H
