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
    bindMatrix = QMatrix4x4();
    bindTransform = QVector3D();
}

Joint::Joint(QString ID0, QString name0)
{
    this->ID = ID0;
    this->name = name0;
    pater = NULL;
    kids = {};
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
    bindMatrix = QMatrix4x4();
    bindTransform = QVector3D();
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
    joints = {};
}


void TraceJoint (Joint* who, int depth){
    QString res = ""; for (int i = 0; i < depth; i++)res += "|  ";
    qDebug() << res + who->name;
    for (int kidInd = 0; kidInd < who->kids.length(); kidInd++)
        TraceJoint (who->kids[kidInd], depth + 1);
    return;
}

void Skeleton::DebugTree()
{
    if (joints.length() <= 0)
    { qDebug () << "No bones;"; return; }
    TraceJoint(&(joints[0]), 0);
}

Rig::Rig()
{
    bindMesh = NULL;
    skeleton = NULL;
    skin = NULL;

}

Rig::Rig(Mesh *mesh, Skeleton *skel, Skin *sk)
{
    bindMesh = mesh;
    skeleton = skel;
    skin = sk;
}
