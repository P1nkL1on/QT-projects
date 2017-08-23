#include "raycast.h"
#include "qvector.h"
#include "stereometry.h"

using namespace RayCast;
using namespace Stereometry;


QImage* RayCast::RenderScene(const Camera *cam, const TestViewer *scene, const TreeSpace::KDTree *tree, const int pixelCount)
{
    QVector<QVector3D> directions = cam->GetCamInfo();
    // 0 - center, 1 - watch to point, 2 - up, 3 - left

    float camHei = 1/Dist(directions[0], directions[2]) , camWid = 1/Dist(directions[0], directions[3]);
    qDebug () << "cam height" << 1/Dist(directions[0], directions[2]) << "cam width "<<1/Dist(directions[0], directions[3]);
    //return NULL;
    QImage* image = new QImage(pixelCount, pixelCount, QImage::Format_RGB666);
    image->fill(Qt::blue);
    GraphicsObject* renderModel = scene->getModel();

    for (int verticalPixel = -pixelCount/2; verticalPixel < pixelCount/2; verticalPixel ++)
        for (int horizontalPixel = -pixelCount/2; horizontalPixel < pixelCount/2; horizontalPixel ++){
            QVector3D* currentRayStart  = new QVector3D();
            QVector3D* currentRayFinish  = new QVector3D();

            *currentRayStart = Summ (directions[0], Summ(
                    Mult ( Resid(directions[2], directions[0]),(verticalPixel / ((float)pixelCount / 2) * camWid * camWid)),
                    Mult ( Resid(directions[3], directions[0]),(-horizontalPixel / ((float)pixelCount / 2) * camHei * camHei))));
            *currentRayFinish = Summ (*currentRayStart, Mult(Resid( directions[1], directions[0] ), 5.0));

            QColor pixelColor = RayCast::RenderPixel(currentRayStart, currentRayFinish, scene, renderModel, tree);
            image->setPixelColor(pixelCount/2 - horizontalPixel - 1, pixelCount/2 - verticalPixel - 1, pixelColor);

            delete currentRayStart;
            delete currentRayFinish;
        }

    return image;
}

QColor RayCast::RenderPixel(const QVector3D *rayStart, const QVector3D *rayFinish, const TestViewer *scene, const GraphicsObject *model, const TreeSpace::KDTree *tree)
{
    unsigned int intersectedPolygonNumber = -1;

    QVector3D* interesction = //intersectWith(currentRayStart, currentRayFinish, intersectedPolygonNumber);
        tree->intersectWith(rayStart, rayFinish, intersectedPolygonNumber, model);
    if (intersectedPolygonNumber!= -1){
        float lightK = RayCast::LightIntense(interesction, interesction, model, scene->getLights(), tree);
        //qDebug() << lightK;
        return QColor((int)(lightK),(int)(lightK),(int)(lightK));
    }

    return Qt::blue;
}

float RayCast::LightIntense(const QVector3D *point, const QVector3D *pointNormal, const GraphicsObject *model,
                   const QVector<LightSourse *> lights, const TreeSpace::KDTree* tree)
{
    float res = 0;
    for (int i = 0; i < lights.length(); i++){
        unsigned int polygonIndex = -1;
        QVector3D* blockingPoint =
                tree->intersectWith(point, &(lights[i]->center), polygonIndex, model);
        if (polygonIndex == -1){
            res += lights[i]->lightPower(Dist(*point, lights[i]->center));
            //qDebug() << res;
        }
    }
    if (res > 255) res = 255;
    return res;
}
