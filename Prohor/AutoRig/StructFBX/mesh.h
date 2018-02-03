#ifndef MESH_H
#define MESH_H
#include "QVector"
#include "Eigen/Core"
#include "derivable.h"

using Eigen::Matrix;
using namespace DerOperations;

class Mesh
{
public:
    Mesh();
    QVector<Matrix<Derivable,1,3>> vertexes;
    QVector<int> polygonIndexes;
    QVector<int> polygonStartIndexes;

    Derivable CompareWithAnotherMesh (const Mesh* anotherMesh) const ;
    QVector<Derivable> CompareWithAnotherMeshCoords(const Mesh *anotherMesh) const;
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // MESH_H
