#include "testautorig.h"


TestAutoRig::TestAutoRig()
{
    targetMeshes = QVector<Mesh*>();
    targetMeshes << NULL;
    bendingRig = NULL;
    angleAdds = QVector<QVector3D>();
}

TestAutoRig::TestAutoRig(Rig *rig, Mesh *mesh)
{
    targetMeshes = QVector<Mesh*>();
    targetMeshes << mesh;
    bendingRig = rig;
    nowRotations = rig->skeleton->getJointsLocalRotations();
    nowRootPose = Matrix<Derivable,1,3>();
    angleAdds = QVector<QVector3D>();
}


void TestAutoRig::ApplyRotations()
{
    float dist = -1;

    QVector<QVector3D> addAngles = QVector<QVector3D>();
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++){
        QVector3D addAngle = QVector3D(0,0,0);

        for (int coord = 0; coord < 3; coord++){
            if (coord > 0)nowRotations[curJoint](0,coord - 1).setPrValue(0); else {if (curJoint > 0)nowRotations[curJoint - 1](0,2).setPrValue(0);}
            nowRotations[curJoint](0,coord).setPrValue(1);
            Derivable res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[0]);
            if (dist < 0) dist = res.getValue();
            addAngle[coord] = res.getProiz();
        }
        qDebug() << "For joint " + QString::number(curJoint) + " add is: " << addAngle;
        addAngles << addAngle * 2;
    }
    nowRotations[nowRotations.length() - 1](0,2).setPrValue(0);

    qDebug() << ">>>>>>Distance : " << dist;
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++)
        nowRotations[curJoint] = nowRotations[curJoint] - Matrix<Derivable,1,3>(addAngles[curJoint].x(),addAngles[curJoint].y(),addAngles[curJoint].z());

//    nowRotations[21] = Matrix<Derivable,1,3>(0,Derivable(0,1), 0);
//    float prevRes = 0;

//    for (int i = 0; i < 210; i++){
//        nowRotations[21] = nowRotations[21] - Matrix<Derivable,1,3>(0, prevRes, 0);
//        Derivable res = bendingRig->CompareWithMeshOnRotates(nowRotations, targetMeshes[0]);
//        qDebug() << "Difference : " + QString::number(res.getValue()) + " (" + QString::number(res.getProiz()) + ")";
//        prevRes = res.getProiz() * 10;
//        qDebug() << "Now Y angle of 21 bone is " + QString::number(nowRotations[21](0,1).getValue()) + " (" + QString::number(nowRotations[21](0,1).getProiz()) + ")";
//    }
}
