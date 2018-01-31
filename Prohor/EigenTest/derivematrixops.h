#ifndef DERIVEMATRIXOPS_H
#define DERIVEMATRIXOPS_H

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

    Matrix<Derivable, 4, 4> MakeDeriveTranslationMatrix (const QVector3D vec);

    Matrix<Derivable, 4, 4> MakeDeriveRotationXMatrix (const float angle0);
    Matrix<Derivable, 4, 4> MakeDeriveRotationZMatrix (const float angle0);
    Matrix<Derivable, 4, 4> MakeDeriveRotationYMatrix (const float angle0);
    Matrix<Derivable, 4, 4> MakeDeriveRotationMatrix (const QVector3D ang);

    void TranslateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const QVector3D vec);
    void RotateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const QVector3D rot);
}

#endif // DERIVEMATRIXOPS_H
