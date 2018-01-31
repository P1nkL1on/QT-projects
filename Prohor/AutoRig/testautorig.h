#ifndef TESTAUTORIG_H
#define TESTAUTORIG_H

#include "StructFBX/rig.h"
#include "QVector"

#include "Eigen/Core"
#include "Derivable/dermatops.h"

class TestAutoRig
{
private:
    QVector<Mesh*> targetMeshes;
    Rig* bendingRig;
    QVector<Matrix<Derivable,1,3>> nowRotations;
    Matrix<Derivable,1,3> nowRootPose;
public:
    TestAutoRig();
    TestAutoRig(Rig* rig, Mesh* mesh);

    void ApplyRotations ();
};

#endif // TESTAUTORIG_H
