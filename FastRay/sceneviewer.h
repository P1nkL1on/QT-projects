#ifndef SCENEVIEWER_H
#define SCENEVIEWER_H

#include "graphicobject.h"
#include "qvector.h"
#include "qpainter.h"
#include "lightsourse.h"

using namespace GraphicObjects;
using namespace RayStruct;


class SceneViewer
{
    QVector<Ray> cameraRays;
    int Width, Height;
public:
    SceneViewer();
    QVector<Primitive*> objects;
    QVector<LightSourse*> light;
    void setCameraSize (const int width, const int height, const QVector3D camCenter, const float step);
    QColor renderPixel (const Ray ray, QPainter *qp, const QVector2D offset, const float scale, int level) const;
    bool renderAndDraw (QPainter* qp, const int resol) const;
};

#endif // SCENEVIEWER_H
