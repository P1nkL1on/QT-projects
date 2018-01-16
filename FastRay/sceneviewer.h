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
    QVector<GraphicObject*> objects;
    QVector<LightSourse*> light;
    void setCameraSize (const int width, const int height, const QVector3D camCenter, const float step);
    QColor renderPixel (const Ray ray) const;
    bool renderAndDraw (QPainter* qp) const;
};

#endif // SCENEVIEWER_H
