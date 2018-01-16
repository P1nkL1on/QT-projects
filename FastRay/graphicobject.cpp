#include "graphicobject.h"

#include "qdebug.h"

using namespace RayStruct;


GraphicObjects::Sphere::Sphere()
{
    center = QVector3D();
    radius = 1;
}

GraphicObjects::Sphere::Sphere(QVector3D cent, float rad)
{
    center = cent;
    radius = rad;
    clr = QColor(200, 100, 0);
}

bool GraphicObjects::Sphere::IntersectWithRay(const RayStruct::Ray *ray, QVector3D &intersection) const
{
    float rayLength = (ray->To() - ray->From()).lengthSquared();
    float rayToCenterDist = (ray->From() - center).lengthSquared() - radius * radius;
    float b = 2 * RayUtils::dot((ray->To() - ray->From()), (ray->From() - center));
    float Disc = b * b - 4 * rayLength * rayToCenterDist;
    if (Disc >= 0){
        if (Disc < 1e-4){
            double d = -b / (2 * rayLength);
            intersection = ray->From() + (ray->To() - ray->From()) * d;
            qDebug() << "Uniq " << intersection;
            return true;
        }
        //
        double d1 = (-b + sqrt(Disc)) / (2 * rayLength),
               d2 = (-b - sqrt(Disc)) / (2 * rayLength);

        QVector3D inter1 = ray->From() + (ray->To() - ray->From()) * d1,
                  inter2 = ray->From() + (ray->To() - ray->From()) * d2;

        intersection = (inter1.distanceToPoint(ray->From()) < inter2.distanceToPoint(ray->From()) ? inter1 : inter2);

        if (intersection.distanceToPoint(ray->From()) > .1)
            return true;
    }
    intersection = ray->To();
    return false;
}

void GraphicObjects::Sphere::Move(QVector3D translate)
{
    center += translate;
}

