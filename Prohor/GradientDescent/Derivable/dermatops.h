#ifndef DERMATOPS_H
#define DERMATOPS_H


#include "QMatrix4x4"
#include "QDebug"
#include "Eigen/Core"
#include "QVector"
#include "derivable.h"

using Eigen::Matrix;
using namespace DerOperations;

namespace DerivableVectorMatrixes {
    void TestTrace();
    Matrix<Derivable, 1, 4> SetDerive4DVector (QVector4D vec);
    Matrix<Derivable, 1, 4> SetDerive4DVector ();
    Matrix<Derivable, 1, 4> SetDerive4DVector (float x, float y, float z, float w);
    Matrix<Derivable, 1, 4> SetDerive4DVector (QVector3D qv3, float w);

    void TraceVector (Matrix<Derivable, 1, 4> Vec);

    void TraceMatrix (Matrix<Derivable, 4, 4> Mat);

    Matrix<Derivable, 4, 4> SetDeriveMatrix ();

    Matrix<Derivable, 4, 4> SetDeriveMatrix (QVector<Derivable> values);

    Matrix<Derivable, 4, 4> SetDeriveMatrix (QMatrix4x4 original);

    Matrix<Derivable, 4, 4> MakeDeriveTranslationMatrix (const Matrix<Derivable, 1,3> vec);

    Matrix<Derivable, 4, 4> MakeDeriveRotationXMatrix (const Derivable angle0);
    Matrix<Derivable, 4, 4> MakeDeriveRotationZMatrix (const Derivable angle0);
    Matrix<Derivable, 4, 4> MakeDeriveRotationYMatrix (const Derivable angle0);
    Matrix<Derivable, 4, 4> MakeDeriveRotationMatrix (const Matrix<Derivable,1,3> ang);

    Matrix<Derivable,1,4> MakeVector4From3 (const Matrix<Derivable, 1,3> vec, const Derivable add);
    Matrix<Derivable,1,3> Summ3and4 (const Matrix<Derivable, 1,3> v3, const Matrix<Derivable, 1,4> v4);
    Derivable SquaredLength (Matrix<Derivable,1,3> vect);
    QVector3D QfromDer3 (const Matrix<Derivable, 1, 3> orig);

    void TranslateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const Matrix<Derivable, 1,3> vec);
    void RotateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const Matrix<Derivable, 1, 3> rot);
}

#endif // DERMATOPS_H
