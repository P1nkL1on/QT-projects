#include "descent.h"
#include "qmath.h"
#include "qdebug.h"
#include "qvector3d.h"
#include "QMatrix3x3"

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

QVector3D Descent::RealProizv() const
{
    QVector3D result(0,0,0);
    float l = currentStep.z(), x = currentStep.x(), y = currentStep.y();

    for (int i = 0; i < modelFinal->vertexCount(); i++){
        float ls = cos(l) * x - sin(l)*y - modelFinal->GetVertex(i).x(),
              rs = sin(l) * x + cos(l)*y - modelFinal->GetVertex(i).y();
        QVector3D add = QVector3D(
                    2 * (ls * cos(l) + rs * sin(l) ),
                    2 * (-ls * sin(l) + rs * cos(l) ),
                    .002 * (ls * (-x * sin(l) - y * cos(l)) + rs * (cos(l) * x - sin(l) * y) )
                    );
        result = QVector3D(result.x() + add.x(), result.y() + add.y(), result.z() + add.z());
    }

    return result;
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
                                                           currentStep.z() + epsilon * (i==2))));
        result[i] = (d1 - d0) / epsilon;
    }
    //result[2] = 555;
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
    //QVector3D proizv = RealProizv();
    QVector3D proizv = CurrentGradientDistValue();;

    TestModel newApproxumate = TranslateAndRotate(modelOriginal, currentStep);
    currentStep = QVector3D(currentStep.x() - proizv.x() * stepMult,currentStep.y() - proizv.y() * stepMult, currentStep.z() - proizv.z() * stepMult);
    lastApproximate = TranslateAndRotate(modelOriginal, currentStep);

    if (DistValue(newApproxumate) < DistValue(lastApproximate))
        stepMult /= 2;

    float nowDist = DistValue(lastApproximate);
    lg0.PushValue(nowDist / 20.0);

    stop = (nowDist < .001);
}

TestModel Descent::TranslateAndRotate(TestModel *originalModel, QVector3D transl) const
{

    QVector<QVector2D> result;
    float x, y;
    for (int i = 0; i < originalModel->vertexCount(); i++){
        x = originalModel->GetVertex(i).x(), y = originalModel->GetVertex(i).y();
        result << QVector2D( cos(transl.z()) * x - sin(transl.z()) * y + transl.x(), sin(transl.z()) * x + cos(transl.z()) * y + transl.y());
    }
    return TestModel(result);
}
