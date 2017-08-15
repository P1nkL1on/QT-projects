#ifndef SCENETOOLS_H
#define SCENETOOLS_H
#include "QVector"

#include "modelloader.h"

using namespace ModelLoader;

namespace SceneTools {
    QVector<unsigned int> triangulateMesh (const QVector<unsigned int> indVert, const QVector<unsigned int> vStart);
}

#endif // SCENETOOLS_H
