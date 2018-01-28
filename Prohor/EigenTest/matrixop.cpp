#include "matrixop.h"

#include "QMatrix4x4"
#include "QDebug"
#include "Eigen/Core"
#include "derivable.h"
#include "qmath.h"

using Eigen::Matrix;
using namespace DerOperations;

MatrixOp::MatrixOp()
{

}



void TraceMatrix (Matrix<Derivable, 4, 4> Mat){
    qDebug() << "Matrix4x4 Derivable (type:Custom";
    for (int i = 0; i < 4; i++){
        QString lin = "";
        for (int j = 0; j < 4; j++)
            lin += (QString::number( Mat(i,j).getValue()) + " (" + QString::number( Mat(i,j).getProiz()) + ")").rightJustified(19, ' ') + " ";
        qDebug() << lin;
    }
    qDebug() << ")";
}

Matrix<Derivable, 4, 4> SetDeriveMatrix (){
    Matrix<Derivable, 4, 4> M = Matrix<Derivable, 4, 4>();
    for (int i = 0; i < 4; i++)
        M(i,i) = Derivable(1);
    return M;
}

Matrix<Derivable, 4, 4> SetDeriveMatrix (QVector<Derivable> values){
    Q_ASSERT(values.length() == 16);
    Matrix<Derivable, 4, 4> M = Matrix<Derivable, 4, 4>();
    M(0,0) = values[0];
    M(0,1) = values[1];
    M(0,2) = values[2];
    M(0,3) = values[3];
    M(1,0) = values[4];
    M(1,1) = values[5];
    M(1,2) = values[6];
    M(1,3) = values[7];
    M(2,0) = values[8];
    M(2,1) = values[9];
    M(2,2) = values[10];
    M(2,3) = values[11];
    M(3,0) = values[12];
    M(3,1) = values[13];
    M(3,2) = values[14];
    M(3,3) = values[15];
    return M;
}

Matrix<Derivable, 4, 4> MakeDeriveTranslationMatrix (const QVector3D vec){
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,3) = vec.x();
    M(1,3) = vec.y();
    M(2,3) = vec.z();
    return M;
}

Matrix<Derivable, 4, 4> MakeDeriveRotationXMatrix (const float angle0){
    float angle = angle0 / 180.0 * M_PI;
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(1,1) = DerOperations::cos(angle); M(1,2) = - DerOperations::sin(angle);
    M(2,1) = DerOperations::sin(angle); M(2,2) = DerOperations::cos(angle);
    return M;
}
Matrix<Derivable, 4, 4> MakeDeriveRotationZMatrix (const float angle0){
    float angle = angle0 / 180.0 * M_PI;
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,0) = DerOperations::cos(angle); M(0,1) = - DerOperations::sin(angle);
    M(1,0) = DerOperations::sin(angle); M(1,1) = DerOperations::cos(angle);
    return M;
}
Matrix<Derivable, 4, 4> MakeDeriveRotationYMatrix (const float angle0){
    float angle = angle0 / 180.0 * M_PI;
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix();
    M(0,0) = DerOperations::cos(angle); M(0,2) = DerOperations::sin(angle);
    M(2,0) =  - DerOperations::sin(angle); M(2,2) = DerOperations::cos(angle);
    return M;
}
Matrix<Derivable, 4, 4> MakeDeriveRotationMatrix (const QVector3D ang){
    return MakeDeriveRotationXMatrix(ang.x()) * MakeDeriveRotationYMatrix(ang.y()) * MakeDeriveRotationZMatrix(ang.z());
}

void TranslateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const QVector3D vec){
    originalMatrix = originalMatrix * MakeDeriveTranslationMatrix(vec);
}
void RotateDeriveMatrix (Matrix<Derivable, 4, 4>& originalMatrix, const QVector3D rot){
    originalMatrix = originalMatrix * MakeDeriveRotationMatrix(rot);
}
void MatrixOp::TestTrace()
{
    QMatrix4x4 Qmat = QMatrix4x4(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16), translateMatrix = Qmat, rotateMatrix = Qmat;

    QVector<Derivable> a = QVector<Derivable>();
    for (int i = 1; i <= 16; i++)
        a << Derivable(i);
    Matrix<Derivable, 4, 4> M = SetDeriveMatrix(a), M2 = M, M3 = M;

    M(0,0) = 1.0;
    qDebug() << Qmat; qDebug() << "*"; qDebug() << Qmat; qDebug() << "="; qDebug() << Qmat * Qmat;  qDebug() << " ";


    TraceMatrix(M);
    qDebug() << "*";
    TraceMatrix(M2);
    qDebug() << "=";
    TraceMatrix(M * M2);


    translateMatrix.translate(3,4,5);
    qDebug() << "";qDebug() << "Translations:";qDebug() << "";
    qDebug() << translateMatrix;

    TranslateDeriveMatrix(M2, QVector3D(3,4,5));
    TraceMatrix(M2);

    QVector3D rotat = QVector3D(12,22,32);//22,32);
    qDebug() << "";qDebug() << "Rotations:";qDebug() << "";

    rotateMatrix.rotate(rotat.x(), 1, 0, 0);
    rotateMatrix.rotate(rotat.y(), 0, 1, 0);
    rotateMatrix.rotate(rotat.z(), 0, 0, 1);
    qDebug() << rotateMatrix;

    //M3 = MakeDeriveRotationMatrix(rotat);
    RotateDeriveMatrix(M3, rotat);
    TraceMatrix(M3);
}
