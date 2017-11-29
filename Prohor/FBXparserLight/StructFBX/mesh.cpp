#include "mesh.h"

Mesh::Mesh()
{
    vertexes = QVector<QVector3D>();
    polygonIndexes = QVector<int>();
    polygonStartIndexes = QVector<int>();
}

float Mesh::CompareWithAnotherMesh(const Mesh *anotherMesh)
{
    Q_ASSERT(vertexes.length() == anotherMesh->vertexes.length());
    float summ = 0;
    for (int vInd = 0; vInd < vertexes.length(); vInd++)
        summ += (vertexes[vInd] - anotherMesh->vertexes[vInd]).lengthSquared() / 100000.0;

    return summ;
}
