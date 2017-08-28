#include "descent.h"
#include "qmath.h"
#include "qdebug.h"
#include "qvector3d.h"
#include "qvector4d.h"
#include "qmatrix4x4.h"

Descent::Descent()
{
    modelFinal = NULL;
    modelOriginal = NULL;
    stop = false;
}

Descent::Descent(TestModel *a, TestModel *b)
{
    modelFinal = b;
    modelOriginal = a;
    stop = false;
}

float Descent::Dist(QVector2D a, QVector2D b) const
{
    return (float)sqrt(pow((double)a.x() - (double)b.x(),2) + pow((double)a.y() - (double)b.y(),2));
}
float Descent::QuadDist(QVector2D a, QVector2D b) const
{
    float distX = a.x() - b.x(),
          distY = a.y() - b.y(),
          res = distX * distX + distY * distY;
    return res;
}

float Descent::DistErrorFunc(TestModel *currentModel) const
{
    float errSumm = 0;
    for (int i = 0; i < currentModel->vertexCount(); i++)
        errSumm += Dist( modelFinal->GetVertex(i), currentModel->GetVertex(i) );
    qDebug() << "Current dist :: " << errSumm;
    return errSumm;
}

void Descent::DrawItSelf(QPainter *qp, int wid, int hei) const
{
    modelOriginal->drawItself(qp, wid, hei, Qt::green);
    modelFinal->drawItself(qp, wid, hei, Qt::black);
    if (lastApproximate.vertexCount() > 0)
        lastApproximate.drawItself(qp, wid, hei, Qt::red);
    lg0.DrawItSelf(qp, 0, 0);
}


float Descent::DistValue (TestModel currentModel) const{
    float errSumm = 0;
    for (int i = 0; i < currentModel.vertexCount(); i++)
        errSumm += QuadDist( modelFinal->GetVertex(i), currentModel.GetVertex(i) );
   // qDebug() << "Current quaddist :: " << errSumm;
    return errSumm;
}

QVector3D Descent::CurrentGradientDistValue() const{
    float epsilon = .00001;
    QVector3D result;
    for (int i = 0; i < 3; i++){
        float d0 = DistValue(TranslateAndRotate(modelOriginal,
                                                QVector3D(currentStep.x(),
                                                          currentStep.y(),
                                                          currentStep.z()))),
              d1 = DistValue(TranslateAndRotate(modelOriginal,
                                                 QVector3D(currentStep.x() + epsilon * (i==0),
                                                           currentStep.y() + epsilon * (i==1),
                                                           currentStep.z() + epsilon * (i==2) * 180)));
        result[i] = (d1 - d0) / epsilon;
    }
    //qDebug() << "Proizvod: " << result;
    return result;
}

float Descent::Module(QVector3D qv) const
{
    return sqrt(qv.x() * qv.x() + qv.y() * qv.y() + qv.z() * qv.z());
}

void Descent::Step()
{
    stepTraRot = QVector3D(.5, .5, .5);
    QVector3D proizv = CurrentGradientDistValue();
    float alp = .0001;
    currentStep = QVector3D(currentStep.x() - proizv.x() * alp,currentStep.y() - proizv.y() * alp, currentStep.z() - proizv.z() * alp);

    lastApproximate = TranslateAndRotate(modelOriginal, currentStep);

    float nowDist = DistValue(lastApproximate);
    lg0.PushValue(nowDist / 10.0);

    stop = (nowDist < .1);
    qDebug() << nowDist;
}

TestModel Descent::TranslateAndRotate(TestModel *originalModel, QVector3D transl) const
{
    QMatrix4x4 rot;
    rot.rotate(transl.z(), QVector3D(0,0,1));

    QVector<QVector2D> result;
    for (int i = 0; i < originalModel->vertexCount(); i++){
        QVector4D was = QVector4D( originalModel->GetVertex(i).x(), originalModel->GetVertex(i).y(), 0 , 1 );
        was = was * rot;
        result << QVector2D(was.x() / was.w() + transl.x(),
                            was.y() / was.w() + transl.y());
    }
    return TestModel(result);
}
