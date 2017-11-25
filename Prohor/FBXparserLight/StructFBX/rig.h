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

    Rig();
    Rig(Mesh *mesh, Skeleton *skel, Skin *sk);


    // drawing to canvas
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
};

#endif // RIG_H
