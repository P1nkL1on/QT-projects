#ifndef SKELETON_H
#define SKELETON_H
#include "structfbx.h"

class Skeleton
{  
    QVector<QVector3D> localRotations;
    void RecursiveApplyLocalRotations(Joint* joint, QVector3D currentRotation);
    int transformesApplied;
public:
    QVector<Joint*> joints;
    Skeleton();
    Skeleton(QVector<Joint*> j);

    void SetNullRotations();

    void DebugTree () const;
    bool CalculateGlobalCoordForEachJoint ();

    QVector<QVector3D> getJointLocalRotations () const;
    QVector<QVector3D> getJointGlobalTranslationsForSkin () const;
    QVector3D getJointCoordByIndex(int index, QVector3D &paterCoord);
    void SetRotation (QVector3D newRotation, int jointInd);
    void SetRotations (QVector<QVector3D> newRotations);

    bool getJointTranslationAndRotation (const int jointIndex, QVector3D& translation, QVector3D& rotation) const;
};

#endif // SKELETON_H
