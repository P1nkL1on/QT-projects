#ifndef MESH_H
#define MESH_H
#include "QVector"
#include "QVector3D"

class Mesh
{
public:
    Mesh();
    QVector<QVector3D> vertexes;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;
};

#endif // MESH_H
