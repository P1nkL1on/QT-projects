#include "testautorig.h"

TestAutoRig::TestAutoRig()
{
    targetMeshes = QVector<Mesh*>();
    targetMeshes << NULL;
    bendingRig = NULL;
}

TestAutoRig::TestAutoRig(Rig *rig, Mesh *mesh)
{
    targetMeshes = QVector<Mesh*>();
    targetMeshes << mesh;
    bendingRig = rig;
    nowRotations = rig->skeleton->getJointsLocalRotations();
    nowRootPose = Matrix<Derivable,1,3>();
}

void TestAutoRig::ApplyRotations()
{
    nowRotations[1] = nowRotations[1] + Matrix<Derivable,1,3>(Derivable(1),Derivable(2),Derivable(3));
    for (int i = 20; i < 22; i++)
        nowRotations[i] = Matrix<Derivable,1,3>(0, 90, 0);
    Derivable res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[0]);
    qDebug() << "Difference in bended rig and current mesh is : " + QString::number(res.getValue()) + " (" + QString::number(res.getValue()) + ")";
}
