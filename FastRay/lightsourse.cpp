#include "lightsourse.h"
#include "QDebug"
LightSourse::LightSourse()
{

    center = QVector3D();
    maxDist = 10.0;
    maxLight = 1.0;
    clr = Qt::white;
}

LightSourse::LightSourse(QVector3D cent, float maxDis, float maxLig, QColor color)
{
    center = cent;
    maxDist = maxDis;
    maxLight = maxLig;
    clr = color;
}

LightSourse::LightSourse(QVector3D cent, float maxDis, float maxLig)
{
    center = cent;
    maxDist = maxDis;
    maxLight = maxLig;
    clr = Qt::white;
}

QColor LightSourse::AddColorOnDist(float dist) const
{
    if (dist > maxDist)
        return Qt::black;
    if (dist <= 0)
        return clr;
    //qDebug() << dist;
    return ColorMult(clr, 1.0 - dist/maxDist);
}

QColor LightSourse::ColorMult(QColor clr, float k)
{
    return QColor((int)(clr.red() * k),(int)(clr.green() * k),(int)(clr.blue() * k));
}

QColor LightSourse::ColorMask(QColor clr, QColor mask)
{
    return QColor((int)(clr.red() * mask.red() / 255.0),(int)(clr.green() * mask.green() / 255.0),(int)(clr.blue() * mask.blue() / 255.0));
}
float min (float a, float b){
    return (a < b)? a : b;
}

QColor LightSourse::ColorAdd(QColor a, QColor b, float bk)
{
    return QColor((int)(min(a.red() + b.red() * bk, 255.0)),(int)(min(a.green() + b.green() * bk,255.0)),(int)(min(a.blue() + b.blue() * bk, 255.0)));
    //qDebug() << b.red() << b.green() << b.blue();
    //return res;
}

bool LightSourse::DrawOn2D(QPainter *qp, const QVector2D offset, const float scale) const
{
    qp->drawEllipse(QPoint((center.x() + offset.x()) * scale,(center.z() + offset.y()) * scale), 5, 5);
    //qp->drawEllipse(QPoint((center.x() + offset.x()) * scale,(center.z() + offset.y()) * scale), (int)(scale*maxDist), (int)(scale*maxDist));
    return true;
}

bool LightSourse::IntersectWithRay(const RayStruct::Ray *ray, QVector3D &intersection) const
{
    return false;
}
