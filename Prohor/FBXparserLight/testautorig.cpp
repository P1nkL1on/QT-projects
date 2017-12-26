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
    for (int i = 20; i < 25; i++)
        nowRotations[i] += QVector3D(2, 1, 3);
    float res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[0]);
    qDebug() << "Difference in bended rig and current mesh is : " + QString::number(res);
}
