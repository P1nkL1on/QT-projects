#include "mesh.h"
#include "Derivable/dermatops.h"
using namespace DerivableVectorMatrixes;

Mesh::Mesh()
{
    vertexes = QVector<Matrix<Derivable,1,3>>();
    polygonIndexes = QVector<int>();
    polygonStartIndexes = QVector<int>();
}

Derivable Mesh::CompareWithAnotherMesh(const Mesh *anotherMesh)
{
    Q_ASSERT(vertexes.length() == anotherMesh->vertexes.length());
    Derivable summ = Derivable(0);
    for (int vInd = 0; vInd < vertexes.length(); vInd++)
        summ = summ + SquaredLength(vertexes[vInd] - anotherMesh->vertexes[vInd]) / 100000.0;

    return summ;
}
