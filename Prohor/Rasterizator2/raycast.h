#ifndef RAYCAST_H
#define RAYCAST_H

#include "testviewer.h"
#include "testkdtree.h"
#include "camera.h"
#include "qvector3d.h"
#include "QImage"

namespace RayCast{

    float LightIntense (const QVector3D* point, const QVector3D* pointNormal,
                        const GraphicsObject* model, const QVector<LightSourse*> lights, const TreeSpace::KDTree* tree );
    QColor RenderPixel ( const QVector3D* rayStart, const QVector3D* rayFinish,
                         const TestViewer* scene, const GraphicsObject* model,  const TreeSpace::KDTree* tree );
    QImage* RenderScene (const Camera* cam, const TestViewer* scene,
                         const TreeSpace::KDTree* tree, const int pixelCount);

}
#endif // RAYCAST_H
