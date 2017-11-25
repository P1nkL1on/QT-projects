#include "skeleton.h"



Skeleton::Skeleton()
{
    localRotations = {};
    joints = {};
}

Skeleton::Skeleton(QVector<Joint *> j)
{
    joints = j;
    localRotations = {};
    for (int lclId = 0; lclId < j.length(); lclId++)
        localRotations << QVector3D();
}

void Skeleton::SetNullRotations()
{
    localRotations = {};
    for (int lclId = 0; lclId < joints.length(); lclId++)
        localRotations << QVector3D();
}


void TraceJoint (const Joint* who, const int depth){
    QString res = ""; for (int i = 0; i < depth; i++)res += "|  ";
    qDebug() << res + who->name;
    for (int kidInd = 0; kidInd < who->kids.length(); kidInd++)
        TraceJoint (who->kids[kidInd], depth + 1);
    return;
}

void Skeleton::DebugTree() const
{
    if (joints.length() <= 0)
    { qDebug () << "No bones;"; return; }
    TraceJoint(joints[0], 0);

    for (int curJoint = 0; curJoint < joints.length(); curJoint++)
        qDebug () << QString::number(curJoint) + ". " + joints[curJoint]->name;
}

void Skeleton::RecursiveApplyLocalRotations(Joint *joint, QVector3D currentRotation)
{
    transformesApplied ++ ;
    //qDebug() << "Applying rotation for " + joint->name << currentRotation;
    QVector3D newRotation;
    if (joint->pater == NULL)
        newRotation = joint->currentRotation + currentRotation;
    else
        newRotation = joint->pater->currentRotation + currentRotation;

    joint->currentTranslation = CommonFuncs::AddDirect(
                (joint->pater == NULL)? joint->currentTranslation : joint->pater->currentTranslation,
                joint->localTranslation, newRotation);
    for (int childId = 0; childId < joint->kids.length(); childId++)
        RecursiveApplyLocalRotations(joint->kids[childId], newRotation);

}

bool EqualSkeletonRotations (QVector<QVector3D> a, QVector<QVector3D> b){
    Q_ASSERT(a.length() == b.length());
    for (int rotInd = 0; rotInd < a.length(); rotInd++)
        if (a[rotInd].x() != b[rotInd].x() || a[rotInd].y() != b[rotInd].y() || a[rotInd].z() != b[rotInd].z())
            return false;
    return true;
}

void Skeleton::CalculateGlobalCoordForEachJoint()
{
    transformesApplied = 0;
    if (EqualSkeletonRotations(getJointLocalRotations(), localRotations))
        return;

    QVector<int> rootInds = {};

    for (int curJoint = 0; curJoint < joints.length(); curJoint++){
        joints[curJoint]->currentTranslation = QVector3D();              // reset XYZ to calculate them further
        joints[curJoint]->currentRotation = localRotations[curJoint];    // now each joint have info about it need angles
        if (joints[curJoint]->pater == NULL) rootInds << curJoint;
    }
    // apply for roots and go further
    Q_ASSERT(rootInds.length() > 0);
    for (int curRootInd = 0; curRootInd < rootInds.length(); curRootInd ++){
        Joint* root = (joints[rootInds[curRootInd]]);
        RecursiveApplyLocalRotations(root, QVector3D());
    }
    qDebug() << "Applied rotation changes for " + QString::number(transformesApplied) + " joints;";
}

QVector<QVector3D> Skeleton::getJointLocalRotations() const
{
    QVector<QVector3D> res;
    for (int curJoint = 0; curJoint < joints.length(); curJoint++)
        res << joints[curJoint]->currentRotation;
    return res;
}



QVector3D Skeleton::getJointCoordByIndex(int index, QVector3D &paterCoord)
{
    if (joints[index]->pater == NULL)
        paterCoord = joints[index]->currentTranslation;
    else
        paterCoord = joints[index]->pater->currentTranslation;
    return joints[index]->currentTranslation;
}

void Skeleton::SetRotation(QVector3D newRotation, int jointInd)
{
    localRotations[jointInd] = newRotation;
}

void Skeleton::SetRotations(QVector<QVector3D> newRotations){
    Q_ASSERT(newRotations.length() == localRotations.length());
    for (int curJ = 0; curJ < localRotations.length(); curJ++)
        localRotations[curJ] = newRotations[curJ];
}

bool Skeleton::getJointTranslationAndRotation(const int jointIndex, QVector3D &translation, QVector3D &rotation) const
{
    translation = joints[jointIndex]->currentTranslation;
    rotation = QVector3D(); Joint* jn = joints[jointIndex];
    do{
        rotation = rotation + jn->currentRotation;
        jn = jn->pater;
    } while ( jn != NULL );
    return true;
}

QVector<QVector3D> Skeleton::getJointGlobalTranslationsForSkin() const
{
    QVector<QVector3D> res;
    for (int curJoint = 0; curJoint < joints.length(); curJoint++)
            res << joints[curJoint]->currentTranslation;
    return res;
}
