#include "structfbx.h"


Mesh::Mesh()
{
    vertexes = QVector<QVector3D>();
    polygonIndexes = QVector<int>();
    polygonStartIndexes = QVector<int>();
}

Joint::Joint()
{
    ID = QString();
    name = QString();
    pater = NULL;
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
}

Joint::Joint(QString ID0, QString name0)
{
    this->ID = ID0;
    this->name = name0;
    pater = NULL;
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
}

Skin::Skin()
{
    jointIndexes = QVector<int>();
    vertIndexes = QVector<QVector<int>>();
    vertWeightes = QVector<QVector<float>>();
}

Skeleton::Skeleton()
{
    //joints[];
}

Rig::Rig()
{
    bindMesh = NULL;
    skeleton = NULL;
    skin = NULL;

}
