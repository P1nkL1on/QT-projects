#ifndef STRUCTFBX_H
#define STRUCTFBX_H

#include "qstring.h"
#include "qvector3d.h"
#include "qvector.h"

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
    Joint joints[];
    Skeleton();
};


struct Rig{
public:
    Mesh* bindMesh;
    Skeleton* skeleton;
    Skin* skin;

    Rig();
};

#endif // STRUCTFBX_H
