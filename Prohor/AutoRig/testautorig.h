#ifndef TESTAUTORIG_H
#define TESTAUTORIG_H

#include "StructFBX/rig.h"
#include "QVector"
#include "qpainter.h"

#include "Eigen/Core"
#include "Derivable/dermatops.h"
#include "Eigen/Dense"

using Eigen::Matrix;
using namespace  DerOperations;

class LossFunction {
public:
    LossFunction(Rig* rig, Mesh* mesh): currentRig(rig), targetMesh(mesh)
    {
        qDebug() << "Distance calculkator aka LossFunction contructed!";
        Q_ASSERT(rig->bindMesh->vertexes.length() == targetMesh->vertexes.length());
    }

    QVector<Derivable> operator()(const QVector<Derivable> params, float& distance) const//QVector<Matrix<Derivable,1,3>> newRotations) const
    {
        Q_ASSERT(params.length() % 3 == 0);
        qDebug() << "Called a method calculating a distances between all vertexes in rig->mesh and target mesh";
        QVector<Matrix<Derivable,1,3>> newRotations = QVector<Matrix<Derivable,1,3>>();
        for (int i = 0 ; i < params.length() / 3; i++)
            newRotations << Matrix<Derivable,1,3>(params[i * 3], params[i * 3 + 1], params[i * 3 + 2]);
        currentRig->skeleton->SetRotations(newRotations);
        currentRig->BendSkinToSkeleton();
        distance = currentRig->bendedMesh->CompareWithAnotherMesh(targetMesh).getValue();
        return currentRig->bendedMesh->CompareWithAnotherMeshCoords(targetMesh);
    }

private:
    Rig* currentRig;
    Mesh* targetMesh;
};

template <typename Function>
bool CalculateJacobian (const QVector<float> currentParams, Matrix<float, -1, -1>& resJacobian, Matrix<float,-1,-1>& F, Function &loss){

    float resDist = -1;
    QVector<Derivable> derCurParams = currentParams,
                       jacobColumn = loss(derCurParams, resDist);

    const unsigned int angCount = currentParams.length(),
                       vertCount = jacobColumn.length();

    resJacobian = Matrix<float,-1,-1>(vertCount, angCount);
    F = Matrix<float,-1,-1>(vertCount, 1);
    for (int curParam = 0; curParam < derCurParams.length(); curParam ++)
    {
        derCurParams[curParam].setPrValue(1);
        jacobColumn = loss(derCurParams, resDist);
        for (int i = 0; i < jacobColumn.length(); i++){
            resJacobian(i, curParam) = jacobColumn[i].getProiz();
            F(i,0) = jacobColumn[i].getValue();
        }
        derCurParams[curParam].setPrValue(0);
    }
}

template <typename Function, typename JacobianCalculator>
QVector<QVector3D> KvaziNewton (Function& func, const QVector<float> params,
                                 const float epsilon, const int maxIterationCount){
    QVector<float> res = params;
    Matrix<float,-1,-1>
            jacobMatrix, F;
    CalculateJacobian(res, jacobMatrix, F, func );
    Matrix<float,-1,-1>
            jacobTrans = Matrix<float, -1, -1>(jacobMatrix.rows(), jacobMatrix.cols()),
            step = Matrix<float,-1,-1>( jacobMatrix.cols(), 1);


    int iterationNumber = 0;
    float currentDistance = 1000000;
    do{
        qDebug() << "Iteration " << ++iterationNumber;

        CalculateJacobian(res, jacobMatrix, F, func );
        jacobTrans = jacobMatrix.transpose();
        step = (jacobTrans * jacobMatrix).colPivHouseholderQr().solve(-jacobTrans * F);

        for (int i = 0; i < jacobMatrix.cols() / 3; i++)
            res[i] = res[i] + .5 * step(i, 0);//+ SetDerive3DVector(.5 * QVector3D(step(i * 3, 0),step(i * 3 + 1, 0),step(i * 3 + 2, 0)));

        func(res, currentDistance);
        qDebug() << "Current distance is now " << currentDistance;
        if (iterationNumber > maxIterationCount) break;
    } while (currentDistance > epsilon);
}

class TestAutoRig : public GraphicsObjectStruct::GraphicsObject
{
private:
    QVector<Rig*> targetMeshes;
    Rig* bendingRig;
    QVector<Matrix<Derivable,1,3>> nowRotations;
    Matrix<Derivable,1,3> nowRootPose;
    QVector<QVector3D> angleAdds;
    float step, prevdist;
    int targMeshInd;
public:
    void ChangeTargetMeshInd (int count);
    void ResetTransofrms ();

    // drawing to canvas
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;


    TestAutoRig();
    TestAutoRig(Rig* rig, QVector<Rig*> mesh);

    float ApplyRotations();
    float JacobianStep ();
    bool Uber ();
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};




#endif // TESTAUTORIG_H
