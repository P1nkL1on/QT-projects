#ifndef RIG_H
#define RIG_H

#include "graphicsobject.h"
#include "mesh.h"
#include "skeleton.h"
#include "skin.h"

class Rig : public GraphicsObjectStruct::GraphicsObject{
public:
    Mesh* bindMesh;
    Skeleton* skeleton;
    Skin* skin;
    Mesh* bendedMesh;

    Rig();
    Rig(Mesh *mesh, Skeleton *skel, Skin *sk);

    QVector3D* cameraCenter;

    // change a mesh w/t a skeleton n skin
    void BendSkinToSkeleton ();
    // drawing to canvas
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
    Derivable CompareWithMeshOnRotates (const QVector<Matrix<Derivable, 1, 3> > newRotations, Mesh* with);
};

#endif // RIG_H
