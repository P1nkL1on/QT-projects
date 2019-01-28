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
private:
    QVector<Ray> cameraRays;
    int Width, Height;
    float Step;
    int maxLevel = 1;
public:
    SceneViewer();
    QVector<Primitive*> objects;
    QVector<LightSourse*> light;
    void setCameraSize (const int width, const int height, const QVector3D camCenter, const float step,
                        const float angle = 0.0, const float perspectiveDistance = 200.0);
    void makeRaysCamera(const QVector3D center, const float angle, const float perspective);
    QColor renderPixel (const Ray ray, QPainter *qp, const QVector2D offset, const float scale, int level) const;
    bool renderAndDraw (QPainter* qp, const int resol) const;
};

#endif // SCENEVIEWER_H
