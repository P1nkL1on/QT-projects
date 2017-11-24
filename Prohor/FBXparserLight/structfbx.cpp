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
    kids = {};
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
}

Joint::Joint(QString ID0, QString name0)
{
    this->ID = ID0;
    this->name = name0;
    pater = NULL;
    kids = {};
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
}

Skin::Skin()
{
    jointIndexes = QVector<int>();
    vertIndexes = QVector<QVector<int>>();
    vertWeightes = QVector<QVector<float>>();
}

void Skin::addInfo(int jointID, QVector<int> Indexes, QVector<float> Weightes)
{
    jointIndexes << jointID;
    vertIndexes << Indexes;
    vertWeightes << Weightes;
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
