#include "testautorig.h"
#include "Derivable/dermatops.h"

using namespace DerivableVectorMatrixes;

TestAutoRig::TestAutoRig()
{
    targetMeshes = QVector<Mesh*>();
    targetMeshes << NULL;
    bendingRig = NULL;
    angleAdds = QVector<QVector3D>();
    step = 2; prevdist = -1;
}

TestAutoRig::TestAutoRig(Rig *rig, Mesh *mesh)
{
    targetMeshes = QVector<Mesh*>();
    targetMeshes << mesh;
    bendingRig = rig;
    nowRotations = rig->skeleton->getJointsLocalRotations();
    nowRootPose = Matrix<Derivable,1,3>();
    angleAdds = QVector<QVector3D>();
    step = 2; prevdist = -1;
}


float TestAutoRig::ApplyRotations()
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
        addAngles << addAngle;
    }
    nowRotations[nowRotations.length() - 1](0,2).setPrValue(0);//


    // changing a step
    if (prevdist > 0){
        if (dist < prevdist) step *= 1.05;
        if (dist > prevdist) step = (step > 2)? 2 : step * .8;
    }
    prevdist = dist;
    float approximateDist = dist + 1;
    do{
        QVector<Matrix<Derivable,1,3>> willBeRotations = QVector<Matrix<Derivable,1,3>>();
        for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++)
            willBeRotations << nowRotations[curJoint] - SetDerive3DVector(addAngles[curJoint] * step);
        approximateDist = bendingRig->CompareWithMeshOnRotates(willBeRotations, targetMeshes[0]).getValue();
        if (approximateDist > dist){ step *= .8; qDebug() << "Approximate citical dist increase! (" << approximateDist << "). Descrease step to " << step;}
    }while(approximateDist > dist);

    float maxGrad = 0; QString lessThen = "", aboveThen = "";
    for (int curJoint = 0; curJoint < addAngles.length(); curJoint++){
        float thisGrad = addAngles[curJoint].length();
        if (thisGrad > maxGrad) maxGrad = thisGrad;
    }
    for (int curJoint = 0; curJoint < addAngles.length(); curJoint++)
        aboveThen += (addAngles[curJoint].length() > maxGrad * .8)? QString::number(curJoint)+", " : "";

    //qDebug() << aboveThen << " are above middle value, graduated with step " << step * 10;
    qDebug() << ">>>>>>Distance : " << dist << ",   step :" << step << "  above half: " << aboveThen;
    for (int curJoint = 0; curJoint < nowRotations.length(); curJoint++){
        float personalStep = step * (1 - .5 * (addAngles[curJoint].length() < maxGrad * .4) + 9 * (addAngles[curJoint].length() > maxGrad * .8));
        nowRotations[curJoint] = nowRotations[curJoint] - SetDerive3DVector(addAngles[curJoint] * step);
    }

    return dist;
}
