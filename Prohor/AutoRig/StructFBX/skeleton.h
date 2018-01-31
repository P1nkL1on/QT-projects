#ifndef SKELETON_H
#define SKELETON_H
#include "structfbx.h"

class Skeleton
{  
    QVector<Matrix<Derivable,1,3>> localRotations;
    void RecursiveApplyLocalRotations(Joint* joint, Matrix<Derivable,1,3> currentRotation);
    int transformesApplied;
public:
    QVector<Joint*> joints;
    Skeleton();
    Skeleton(QVector<Joint*> j);

    // local
    void SetNullRotations();

    void DebugTree () const;
    bool CalculateGlobalCoordForEachJoint ();
    bool CalculateGlobalCoordForEachJointMatrix();

    QVector<Matrix<Derivable,1,3>> getJointsLocalRotations () const;
    QVector<Matrix<Derivable,1,3>> getJointsGlobalTranslationsForSkin () const;
    Matrix<Derivable,1,3> getJointCoordByIndex(int index, Matrix<Derivable,1,3> &paterCoord);
    // local!
    void SetRotation (Matrix<Derivable,1,3> newRotation, int jointInd);
    void SetRotations (QVector<Matrix<Derivable,1,3>> newRotations);

    // return local!
    bool getJointTranslationAndRotation (const int jointIndex, Matrix<Derivable,1,3>& translation, Matrix<Derivable,1,3>& rotation) const;
};

#endif // SKELETON_H
