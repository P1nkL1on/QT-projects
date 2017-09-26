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

#endif // CLUSTER_H
