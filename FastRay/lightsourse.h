#ifndef LIGHTSOURSE_H
#define LIGHTSOURSE_H

#include "QColor"
#include "graphicobject.h"
using namespace GraphicObjects;

class LightSourse : public GraphicObject
{

    float maxDist;
public:
    QVector3D center;
    float maxLight;

    LightSourse();
    LightSourse (QVector3D cent, float maxDis, float maxLig, QColor color);
    LightSourse (QVector3D cent, float maxDis, float maxLig);
    QColor AddColorOnDist (float dist) const;
    static QColor ColorMult (QColor original, float b);
    static QColor ColorMask (QColor original, QColor mask);
    static QColor ColorAdd (QColor a, QColor b, float k);
    bool DrawOn2D (QPainter* qp, const QVector2D offset, const float scale) const override;
    bool IntersectWithRay (const RayStruct::Ray* ray, QVector3D &intersection) const override;
};

#endif // LIGHTSOURSE_H
