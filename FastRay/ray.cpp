#include "ray.h"



RayStruct::Ray::Ray(){

}

RayStruct::Ray::Ray(QVector3D start, QVector3D finish)
{
    from = start;
    to = finish;
}

QVector3D RayStruct::Ray::From() const
{
    return from;
}

QVector3D RayStruct::Ray::To() const
{
    return to;
}
