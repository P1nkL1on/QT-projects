#include "raycast.h"
#include "qvector.h"
#include "stereometry.h"

using namespace RayCast;
using namespace Stereometry;


QImage* RayCast::RenderScene(const Camera *cam, const TestViewer *scene, const TreeSpace::KDTree *tree, const int pixelCount, Model *&mod2)
{
    int scale = 3;
    QVector<QVector3D> directions = cam->GetCamInfo();
    // 0 - center, 1 - watch to point, 2 - up, 3 - left

    float camHei = 1/Dist(directions[0], directions[2]) , camWid = 1/Dist(directions[0], directions[3]);
    qDebug () << "cam height" << 1/Dist(directions[0], directions[2]) << "cam width "<<1/Dist(directions[0], directions[3]);
    //return NULL;
    QImage* image = new QImage(pixelCount * scale, pixelCount * scale, QImage::Format_RGB666);
    image->fill(Qt::blue);
    GraphicsObject* renderModel = scene->getModel();

    Model* mod = new Model(); int gett = 0;

    for (int verticalPixel = -pixelCount/2; verticalPixel < pixelCount/2; verticalPixel ++, qDebug() << verticalPixel << "/" << pixelCount / 2)
        for (int horizontalPixel = -pixelCount/2; horizontalPixel < pixelCount/2; horizontalPixel ++){
            QVector3D* currentRayStart  = new QVector3D();
            QVector3D* currentRayFinish  = new QVector3D();

            *currentRayStart = Summ (directions[0], Summ(
                    Mult ( Resid(directions[2], directions[0]),(verticalPixel / ((float)pixelCount / 2) * camWid * camWid)),
                    Mult ( Resid(directions[3], directions[0]),(-horizontalPixel / ((float)pixelCount / 2) * camHei * camHei))));
            *currentRayFinish = Summ (*currentRayStart, Mult(Resid( directions[1], directions[0] ), 5.0 * camHei));

            QVector3D inter;
            QColor pixelColor = RayCast::RenderPixel(currentRayStart, currentRayFinish, scene, renderModel, tree, inter);
            for (int i = 0; i < scale; i++)for(int j = 0; j < scale; j++)
                image->setPixelColor((pixelCount/2 - horizontalPixel - 1) * scale + i, (pixelCount/2 - verticalPixel - 1) * scale + j, pixelColor);

            delete currentRayStart;
            delete currentRayFinish;

            mod->vertexes << inter;
            if (gett > 0)mod->polygon_start << gett * 3;
            gett++;
            mod->polygon_vertex_indexes << gett << gett << gett;
        }

    return image;
}

QVector3D* RayCast::Ballicentrate (const QVector<QVector3D> verts, const QVector3D ballicenter){

//    Q_ASSERT ((verts.length() == 3));
//    Q_ASSERT (ballicenter.x() > -.001 && ballicenter.y() > -.001 && ballicenter.z() > -.001 &&
//              ballicenter.x() < 1.001 && ballicenter.y() < 1.001 && ballicenter.z() < 1.001);

    QVector3D* normal = new QVector3D();
    *normal = QVector3D(verts[0].x() * ballicenter.x() + verts[1].x() * ballicenter.y() + verts[2].x() * ballicenter.z(),
                        verts[0].y() * ballicenter.x() + verts[1].y() * ballicenter.y() + verts[2].y() * ballicenter.z(),
                        verts[0].z() * ballicenter.x() + verts[1].z() * ballicenter.y() + verts[2].z() * ballicenter.z());
    return normal;
}

QColor RayCast::RenderPixel(const QVector3D *rayStart, const QVector3D *rayFinish, const TestViewer *scene,
                            const GraphicsObject *model, const TreeSpace::KDTree *tree, QVector3D &intersectionReturnXYZ)
{
    unsigned int intersectedPolygonNumber = -1;

    QVector3D* interesction =
        tree->intersectWith(rayStart, rayFinish, intersectedPolygonNumber, model);
    intersectionReturnXYZ = *rayFinish;

    if (intersectedPolygonNumber!= -1){
        // нашли пересечение с полигоном
        QVector3D* intersectionPointXYZ = Ballicentrate(model->GetVertexes(intersectedPolygonNumber), *interesction);
        QVector3D* interNormalPointXYZ = Ballicentrate(model->GetVertexNormals(intersectedPolygonNumber), *interesction);
        // нашли точку в координатах хуз и нормаль
        intersectionReturnXYZ = *intersectionPointXYZ;
        // освещенность этой точки
        float lightK =
                RayCast::LightIntense(intersectionPointXYZ, interNormalPointXYZ, model, scene->getLights(), tree);

        QColor pixelColor = QColor((int)(lightK),(int)(lightK),(int)(lightK));
        if ( model->IsMirror(intersectedPolygonNumber) > 0 ){
            // рекурсивно выпускаем отраженный луч
            QVector3D* reflectedXYZ = new QVector3D();
            *reflectedXYZ = Reflect(rayStart, intersectionPointXYZ, interNormalPointXYZ);
            //qDebug () << "Reflect to " << *reflectedXYZ;
            QColor reflectColor = RayCast::RenderPixel(intersectionPointXYZ, reflectedXYZ, scene, model, tree, intersectionReturnXYZ);

            return RayCast::ColorSumm(pixelColor, reflectColor, (float)(1 - model->IsMirror(intersectedPolygonNumber)));
        }
        // в противном случае возвращаем просто цвет пикселя
        return pixelColor;
    }
    return Qt::blue;
}

float RayCast::LightIntense(const QVector3D *point, const QVector3D *pointNormal, const GraphicsObject* model,
                   const QVector<LightSourse *> lights, const TreeSpace::KDTree* tree)
{
    float res = 0;
    for (int i = 0; i < lights.length(); i++){
        unsigned int polygonIndex = -1;
        QVector3D* blockingPoint =
                tree->intersectWith(point, &(lights[i]->center), polygonIndex, model);
        if (polygonIndex == -1){
            double angle = (Angle(lights[i]->center, *point, *pointNormal)) / (M_PI) * 2.0 - 1.0;
            res +=  lights[i]->lightPower(Dist(*point, lights[i]->center)) * ((angle > 0)? angle : 0);
        }
    }
    if (res > 255) res = 255;
    return res;
}


QColor RayCast::ColorSumm(QColor A, QColor B, float koeff)
{
    return QColor((int)(A.red() * koeff + B.red() * (1 - koeff)),
                  (int)(A.green() * koeff + B.green() * (1 - koeff)),
                  (int)(A.blue() * koeff + B.blue() * (1 - koeff)));
}

