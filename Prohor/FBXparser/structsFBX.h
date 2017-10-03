#ifndef CLUSTER_H
#define CLUSTER_H

#include "QString"
#include "QVector"
#include "QMatrix4x4"
#include "QMatrix3x3"


struct Cluster
{
public:
    Cluster();
    QString ID;
    QVector<int> indexes;
    QVector<float> weights;

    QVector<float> Transform;
    QVector<float> LinkTransform;
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
    QString name;
    QVector3D translation;
    QVector3D rotation;
    LimbNode* pater;
    //from limbs
    float lengthFromAttribute;
    //bind read
    QMatrix4x4 BindMatrix;
    QMatrix4x4 RotatMatrix;
    QMatrix4x4 Transform;
    QMatrix4x4 LinkTransform;
    bool correctTransformsCluster;
    // cluster add
    QVector<int> indexes;
    QVector<float> weights;
};

#endif // CLUSTER_H
