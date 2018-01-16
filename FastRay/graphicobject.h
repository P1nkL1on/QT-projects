#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H

#include "ray.h"
#include "QColor"

namespace  GraphicObjects {
    class GraphicObject
    {
    public:
        QColor clr;
        virtual bool IntersectWithRay (const RayStruct::Ray* ray, QVector3D &intersection) const = 0;
        //virtual RayStruct::Ray NormalInPoint (const QVector3D inPoint) const;
    };

    class Primitive : public GraphicObject{
    public:

    };

    class Sphere : public Primitive{
    private:
        QVector3D center;
        float radius;
    public:
        Sphere ();
        Sphere (QVector3D cent, float rad);
        bool IntersectWithRay (const RayStruct::Ray* ray, QVector3D &intersection) const override;
        void Move(QVector3D translate);
    };
}

#endif // GRAPHICOBJECT_H
