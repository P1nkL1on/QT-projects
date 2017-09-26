#ifndef CLUSTER_H
#define CLUSTER_H

#include "QString"
#include "QVector"
#include "QMatrix4x4"


struct Cluster
{
public:
    Cluster();
    QString ID;
    QVector<int> indexes;
    QVector<float> weights;
    //QMatrix4x4 Transform;
    //QMatrix4x4 LinkTransform;
};

struct LimbNodeAttribute
{
public:
    LimbNodeAttribute();
    QString ID;
    float length;
};

struct LimbNode{
public:
    LimbNode();
    QString ID;
    QVector3D translation;
    QVector3D rotation;
    LimbNode* pater;
    float lengthFromAttribute;
    QVector<float> BindMatrix;
};

#endif // CLUSTER_H
