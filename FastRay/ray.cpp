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

QVector3D RayStruct::Ray::ray() const
{
    return to - from;
}

float RayStruct::Ray::GetAngleBetween(const RayStruct::Ray &another)
{
    return (ray().dotProduct(ray(), another.ray()) / (ray().length() * another.ray().length()));
}
