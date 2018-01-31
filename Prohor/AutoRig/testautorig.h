#ifndef TESTAUTORIG_H
#define TESTAUTORIG_H

#include "StructFBX/rig.h"
#include "QVector"

class TestAutoRig
{
private:
    QVector<Mesh*> targetMeshes;
    Rig* bendingRig;
    QVector<QVector3D> nowRotations;
    QVector3D nowRootPose;
public:
    TestAutoRig();
    TestAutoRig(Rig* rig, Mesh* mesh);

    void ApplyRotations ();
};

#endif // TESTAUTORIG_H
