#ifndef STRUCTFBX_H
#define STRUCTFBX_H

#include "qstring.h"
#include "qvector3d.h"
#include "qvector.h"
#include "QMatrix4x4"

#include "graphicsobject.h"


struct Mesh{
public:
    Mesh();
    QVector<QVector3D> vertexes;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;
};

struct Joint{
public:
    QString ID;
    QString name;
    Joint* pater;
    QVector<Joint*> kids;
    QVector3D currentTranslation;
    QVector3D currentRotation;
    QMatrix4x4 bindMatrix;
    QVector3D bindTransform;

    Joint();
    Joint(QString ID0, QString name0);
};

struct Skin{
    QVector<int> jointIndexes;
    QVector<QVector<int>> vertIndexes;
    QVector<QVector<float>> vertWeightes;
public:
    Skin();
    bool getInfoByJointIndex (int jointID, QVector<int> &outIndexes, QVector<float> &outWeightes) const;
    void addInfo (int jointID, QVector<int> Indexes, QVector<float> Weightes );
};

struct Skeleton{
public:
    QVector<Joint> joints;
    Skeleton();

    void DebugTree ();
    QVector3D getJointCoordByIndex(int index, QVector3D &paterCoord);
};


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

#endif // STRUCTFBX_H
