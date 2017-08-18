#ifndef PHYSICOBJECT_H
#define PHYSICOBJECT_H
#include "QVector2D"

class PhysicObject
{
private:
    QVector2D CenterNode;
    float Height;
    float Width;
    float Mass;
    float VelocityBacking;
    float GroundBacking;

    QVector2D SpeedSelf;
    QVector2D SpeedForced;
public:
    PhysicObject();
};

#endif // PHYSICOBJECT_H
