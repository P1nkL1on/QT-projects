#ifndef STRUCTFBX_H
#define STRUCTFBX_H

#include "qstring.h"
#include "qvector.h"
#include "Eigen/Core"
#include "derivable.h"

using Eigen::Matrix;
using namespace DerOperations;

struct Joint{
public:
    QString ID;
    QString name;
    Joint* pater;
    QVector<Joint*> kids;
    Matrix<Derivable,1,3> localTranslation;
    Matrix<Derivable,1,3> currentTranslation;
    Matrix<Derivable,1,3> currentRotation;
    Matrix<Derivable,4,4> bindMatrix;
    Matrix<Derivable,1,3> bindTransform;

    Matrix<Derivable,4,4> localTransformMatrix;
    Matrix<Derivable,4,4> globalTransformMatrix;

    Joint();
    Joint(QString ID0, QString name0);

    void RecaulculateLocalTransformMatrix ();
    void ResetGlobalTransformMatrix();
    void CalculateGlobalTransformMatrix();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};


struct AttendedVertex{
public:
    QVector<int> jointIndexs;
    QVector<float> weights;
    QVector<Matrix<Derivable,1,3>> localJointCoords;
    AttendedVertex();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

namespace CommonFuncs {
    Matrix<Derivable,4,4> GetNormalRotateMatrix (const Matrix<Derivable,1,3> rotatXYZ);
    Matrix<Derivable,1,3> AddDirectMatrx (const Matrix<Derivable,1,3> localTransform, const Matrix<Derivable,4,4> transform);
    Matrix<Derivable,1,3> AddDirect (const Matrix<Derivable,1,3> to, const Matrix<Derivable,1,3> Transform, const Matrix<Derivable,1,3> Rotation);
    Matrix<Derivable,1,3> AddDirectWtParent (const Matrix<Derivable,1,3> to, const Matrix<Derivable,1,3> Transform, const Matrix<Derivable,1,3> wasRotation, const Matrix<Derivable,1,3> addRotation );
}



#endif // STRUCTFBX_H
