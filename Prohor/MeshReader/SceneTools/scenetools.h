#ifndef SCENETOOLS_H
#define SCENETOOLS_H
#include "QVector"

#include "modelloader.h"

namespace SceneTools {
    QVector<unsigned int> triangulateMesh (const QVector<unsigned int> indVert, const QVector<unsigned int> vStart);
    QVector<QVector3D> calculateNormals(const QVector<QVector3D> verts, const QVector<unsigned int> polygonVertexIndexes,
                                        const QVector<unsigned int> polygonStartIndexes);
}

#endif // SCENETOOLS_H
