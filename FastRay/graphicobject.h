#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H

#include "ray.h"
#include "QColor"
#include "qpainter.h"
#include "qvector2d.h"

namespace  GraphicObjects {
    class GraphicObject
    {
    public:
        QColor clr;
        virtual bool IntersectWithRay (const RayStruct::Ray* ray, QVector3D &intersection) const = 0;
        virtual bool DrawOn2D (QPainter* qp, const QVector2D offset, const float scale) const = 0;
        //virtual RayStruct::Ray NormalInPoint (const QVector3D inPoint) const;
    };

    class Primitive : public GraphicObject{
    public:
        float mirrority;
        virtual RayStruct::Ray GetNormalRay (const QVector3D from) const = 0;
    };

    class Sphere : public Primitive{
    private:
        QVector3D center;
        float radius;
    public:
        Sphere ();
        Sphere (QVector3D cent, float rad);
        Sphere (QVector3D cent, float rad, QColor color);
        bool IntersectWithRay (const RayStruct::Ray* ray, QVector3D &intersection) const override;
        bool DrawOn2D (QPainter* qp, const QVector2D offset, const float scale) const override;
        RayStruct::Ray GetNormalRay (const QVector3D from) const override;
        void Move(QVector3D translate);

    };
}

#endif // GRAPHICOBJECT_H
