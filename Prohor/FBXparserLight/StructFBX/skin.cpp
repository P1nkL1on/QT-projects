#include "skin.h"

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

void Skin::GenerateAttends(const QVector<QVector3D> vertexes, QVector<QVector3D> globalJointCoords)
{
    AttendedVertex vts[vertexes.length()];
    for (int v = 0; v < vertexes.length(); v++)
        vts[v] = AttendedVertex();

    for (int curJoint = 0; curJoint < jointIndexes.length(); curJoint++)
        for (int vertInd = 0; vertInd < vertIndexes[curJoint].length(); vertInd ++){
            vts[vertIndexes[curJoint][vertInd]].jointIndexs << curJoint;
            vts[vertIndexes[curJoint][vertInd]].weights << vertWeightes[curJoint][vertInd];
        }
    for (int v = 0; v < vertexes.length(); v++)
        vertAttends << vts[v];
//  for (int v = 0; v < vertCount; v++)
//    {
//        QString res = QString::number(v) + ". ";
//        for (int j = 0; j < vts[v].jointIndexs.length(); j++)
//            res += " " + QString::number(vts[v].jointIndexs[j]) + "==" + QString::number(vts[v].weights[j]);
//    }
}
