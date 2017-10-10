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

struct AnimCurve{
public:
    QVector<float> values;
    QString ID;
    //short typ;  // 0 - XTRAN 1 - YTAN 2 - ZTRAN 3 - XROT 4 - YROT 5 - ZROT
    AnimCurve();
};

struct AnimNode {
public:
    QVector<float> xvalues;
    QVector<float> yvalues;
    QVector<float> zvalues;
    QString ID;
    short typ;  // 0 - TRNASLATION  3 - ROTATION
    AnimNode();
};


struct LimbNode{
public:
    LimbNode();
    QString ID;
    QString name;

    QVector3D translation;
    QVector3D rotation;

    AnimNode* animTrnaslation;
    AnimNode* animRotation;

    LimbNode* pater;
    //from limbs
    float lengthFromAttribute;
    //bind read
    QMatrix4x4 BindMatrix;
    QMatrix4x4 RotatMatrix;
    //
    QMatrix4x4 Transform;
    QMatrix4x4 LinkTransform;
    //bool correctTransformsCluster;
    // cluster add
    QVector<int> indexes;
    QVector<float> weights;
};

#endif // CLUSTER_H
