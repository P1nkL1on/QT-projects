#ifndef RAYCAST_H
#define RAYCAST_H

#include "testviewer.h"
#include "testkdtree.h"
#include "camera.h"
#include "qvector3d.h"
#include "QImage"
#include "modelloader.h"
#include "qthread.h"

namespace RayCast{
    QColor ColorSumm (QColor A, QColor B, float koeff);
    QColor ColorMult (QColor mainColor, float shadow);

    QVector3D* Ballicentrate (const QVector<QVector3D> verts, const QVector3D ballicenter);

    QVector2D* Ballicentrate2D (const QVector<QVector2D> verts, const QVector3D ballicenter);

    float LightIntense (const QVector3D* point, const QVector3D* pointNormal,
                        const GraphicsObject* model, const QVector<LightSourse*> lights, const TreeSpace::KDTree* tree );
    QColor RenderPixel (const QVector3D* rayStart, const QVector3D* rayFinish,
                         const TestViewer* scene, const GraphicsObject* model,
                         const TreeSpace::KDTree* tree, QVector3D &intersectionReurnXYZ );
    QImage* RenderScene (const Camera* cam, const TestViewer* scene,
                         const TreeSpace::KDTree* tree, const int pixelCount,
                         ModelStructs::Model* &intersectionPoints);

    class DrawPixelThread : public QThread
    {
        Q_OBJECT
    private:
        void run();
    };
}
#endif // RAYCAST_H
