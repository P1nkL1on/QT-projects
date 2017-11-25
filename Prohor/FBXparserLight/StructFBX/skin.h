#ifndef SKIN_H
#define SKIN_H
#include "QVector"
#include "structfbx.h"


class Skin
{
    QVector<int> jointIndexes;
    QVector<QVector<int>> vertIndexes;
    QVector<QVector<float>> vertWeightes;

public:
    Skin();
    bool getInfoByJointIndex (int jointID, QVector<int> &outIndexes, QVector<float> &outWeightes) const;
    void addInfo (int jointID, QVector<int> Indexes, QVector<float> Weightes );
    void GenerateAttends (const QVector<QVector3D> vertexes, QVector<QVector3D> globalJointCoords);
    // ... final view ...
    QVector<AttendedVertex> vertAttends;
};

#endif // SKIN_H
