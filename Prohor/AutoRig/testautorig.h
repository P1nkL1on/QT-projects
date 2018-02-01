#ifndef TESTAUTORIG_H
#define TESTAUTORIG_H

#include "StructFBX/rig.h"
#include "QVector"
#include "qpainter.h"

#include "Eigen/Core"
#include "Derivable/dermatops.h"

class TestAutoRig
{
private:
    QVector<Mesh*> targetMeshes;
    Rig* bendingRig;
    QVector<Matrix<Derivable,1,3>> nowRotations;
    Matrix<Derivable,1,3> nowRootPose;
    QVector<QVector3D> angleAdds;
    float step, prevdist;
public:
    TestAutoRig();
    TestAutoRig(Rig* rig, Mesh* mesh);

    float ApplyRotations();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif // TESTAUTORIG_H
