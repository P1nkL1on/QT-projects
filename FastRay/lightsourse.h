#ifndef LIGHTSOURSE_H
#define LIGHTSOURSE_H

#include "QVector3D"
#include "QColor"

class LightSourse
{

    float maxDist;
    QColor clr;
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
};

#endif // LIGHTSOURSE_H
