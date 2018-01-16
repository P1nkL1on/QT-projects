#include "sceneviewer.h"
#include "qdebug.h"

SceneViewer::SceneViewer()
{
    cameraRays = QVector<Ray>();
}

void SceneViewer::setCameraSize(const int width, const int height, const QVector3D camCenter, const float step)
{
    Width = width;
    Height = height;

    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++)
            cameraRays << Ray(camCenter + step * (QVector3D(1,0,0) * (j - Width / 2) + QVector3D(0,-1,0) * ( i - Height / 2)),
                              camCenter + QVector3D(0,0, 100));
}

QColor SceneViewer::renderPixel(const Ray ray) const
{
    QVector<QVector3D> intersectPoints;
    QVector<int> intersectIndexes;

    for (int i = 0; i < objects.length(); i++){
        QVector3D res;
        bool intersected = objects[i]->IntersectWithRay(&ray, res);
        if (intersected)
        {
            intersectPoints << res;
            intersectIndexes << i;
        }
    }

    // SORT ON DIST and decide which one is intersected
    if (intersectIndexes.length() > 0){
        float minDist = (ray.From() - ray.To()).length() + 1;
        int bestInd = -1, bestI = -1;
        for (int i = 0; i < intersectIndexes.length(); i++)
        {
            float dist = intersectPoints[i].distanceToPoint(ray.From());
            if (dist < minDist)
            {bestInd = intersectIndexes[i];bestI = i; minDist = dist;}
        }
        if (bestInd >= 0){
            QColor summ = Qt::black;
            for (int i = 0; i < light.length(); i++){
                QVector3D lightBlock; bool isVisible = true;
                // check if light is visible from a contact point

                Ray R = Ray(intersectPoints[bestI], light[i]->center);
                for (int j = 0; j < objects.length(); j++){
                    bool intersected = objects[j]->IntersectWithRay(&R, lightBlock);
                    if (intersected && lightBlock.distanceToPoint(R.To()) > .1){isVisible = false; break;}
                }
                //
                if (!isVisible)
                    continue;
                //
                float distToLight = intersectPoints[bestI].distanceToPoint(light[i]->center);
                summ = LightSourse::ColorAdd(summ, light[i]->AddColorOnDist(distToLight), light[i]->maxLight);
            }
            return LightSourse::ColorMask(objects[bestInd]->clr, summ);
        }

    }
    return QColor(5, 5, 200);
}

bool SceneViewer::renderAndDraw(QPainter *qp) const
{
    int wid = 4;
    QPen pen = QPen();
    pen.setWidth(wid);
    for (int i = 0; i < Height; i++)
        for (int j = 0; j < Width; j++){
            int rayInd = i + j * Width;
            if (rayInd >= cameraRays.length())
                return false;
            //qDebug() << rayInd << i << j;
            pen.setColor(renderPixel(cameraRays[rayInd]));
            qp->setPen(pen);
            qp->drawPoint(20 + i * wid, 20 + j * wid);
        }
    return true;
}
