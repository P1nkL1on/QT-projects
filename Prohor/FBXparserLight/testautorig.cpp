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
    nowRootPose = QVector3D();
}

void TestAutoRig::ApplyRotations()
{

    nowRotations[22] = nowRotations[22] + QVector3D(0, 10, 0);
    float res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[0]);
    qDebug() << "Difference in bended rig and current mesh is : " + QString::number(res);
}
