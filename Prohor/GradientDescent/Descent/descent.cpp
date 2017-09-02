#include "descent.h"
#include "qmath.h"
#include "qdebug.h"
#include "qvector3d.h"
#include "QMatrix3x3"

#include "Linegraphics/linegraphics.h"


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
    lastApproximate = *modelOriginal;
}

void Descent::DrawItSelf(QPainter *qp, int wid, int hei) const
{
    lg0.DrawItSelf(qp, 10, hei - 110, wid);

    if (modelFinal == NULL || modelOriginal == NULL) return;

    modelFinal->drawFromItTo(qp, wid, hei, &lastApproximate);
    modelOriginal->drawItself(qp, wid, hei, Qt::green);
    modelFinal->drawItself(qp, wid, hei, Qt::black);

    if (lastApproximate.vertexes.length() > 0)
        lastApproximate.drawItself(qp, wid, hei, Qt::red);
}

void Descent::Step()
{
    QVector<QPair<Derivable, Derivable>> orig = modelOriginal->toDerivableVector(),
                                         final = modelFinal->toDerivableVector();
    QVector<Derivable> getedDeriv = Derivable::AutoDiff(orig, final, currentStep);
    QVector3D currentAdd = QVector3D(getedDeriv[0].getProiz(), getedDeriv[1].getProiz(), getedDeriv[2].getProiz());
    qDebug() << currentAdd;
    double stepMult = .001;
    currentStep = QVector3D(currentStep.x() - currentAdd.x() * stepMult,currentStep.y() - currentAdd.y() * stepMult, currentStep.z() - currentAdd.z() * stepMult * .1);

    lastApproximate = TranslateAndRotate(modelOriginal, currentStep);

    double nowDist = CurrentDist(modelOriginal, modelFinal, currentStep);
    lg0.PushValue(nowDist / 20.0);
    stop = (nowDist < .001);
}

TestModel Descent::TranslateAndRotate(TestModel *originalModel, QVector3D transl)
{
    QVector<QVector2D> result;
    double x, y;
    for (int i = 0; i < originalModel->vertexes.length(); i++){
        x = originalModel->vertexes[i].x(), y = originalModel->vertexes[i].y();
        result << QVector2D( cos(transl.z()) * x - sin(transl.z()) * y + transl.x(), sin(transl.z()) * x + cos(transl.z()) * y + transl.y());
    }
    return TestModel(result);
}


double Descent::CurrentDist(TestModel *originalModel, TestModel *finalModel, QVector3D derTransform)
{
    QVector<double> derVector = {derTransform.x(), derTransform.y(), derTransform.z()};
    return Derivable::CurDist(originalModel->todoubleVector(), finalModel->todoubleVector(), derVector);
}



//double Descent::Dist(QVector2D a, QVector2D b) const
//{
//    return (double)sqrt(pow((double)a.x() - (double)b.x(),2) + pow((double)a.y() - (double)b.y(),2));
//}
//double Descent::QuadDist(QVector2D a, QVector2D b) const
//{
//    double distX = a.x() - b.x(),
//          distY = a.y() - b.y(),
//          res = distX * distX + distY * distY;
//    return res;
//}

//double Descent::DistErrorFunc(TestModel *currentModel) const
//{
//    double errSumm = 0;
//    for (int i = 0; i < currentModel->vertexCount(); i++)
//        errSumm += Dist( modelFinal->GetVertex(i), currentModel->GetVertex(i) );
//    qDebug() << "Current dist :: " << errSumm;
//    return errSumm;
//}




//double Descent::DistValue (TestModel currentModel) const{
//    double errSumm = 0;
//    for (int i = 0; i < currentModel.vertexCount(); i++)
//        errSumm += QuadDist( modelFinal->GetVertex(i), currentModel.GetVertex(i) );
//   // qDebug() << "Current quaddist :: " << errSumm;
//    return errSumm;
//}

//QVector<Derivable> Descent::AutoDiffThisShit (TestModel* originalModel, TestModel* finalModel, QVector3D transform) const {

//    QVector<Derivable> res = {Derivable(), Derivable(), Derivable()};

//    for (int coord = 0 ; coord < 3; coord++){
//        QVector<Derivable> derTransform = {Derivable(transform.x()), Derivable(transform.y()), Derivable(transform.z())};
//        derTransform[coord].setPrValue(1);  // make a variable x y l

//        for (int i = 0; i < originalModel->vertexCount(); i++){
//            Derivable xO = Derivable(originalModel->GetVertex(i).x()),
//                      yO = Derivable(originalModel->GetVertex(i).y()),
//                      xF = Derivable(finalModel->GetVertex(i).x()),
//                      yF = Derivable(finalModel->GetVertex(i).y());
//            res[coord] = res[coord]
//                     +Derivable::Dpow (xO * Derivable::Dcos(derTransform[2]) - yO * Derivable::Dsin(derTransform[2]) + derTransform[0] - xF,2)
//                     +Derivable::Dpow (xO * Derivable::Dsin(derTransform[2]) + yO * Derivable::Dcos(derTransform[2]) + derTransform[1] - yF,2);
//        }
//    }
//    return res;
//}

//QVector3D Descent::RealProizv() const
//{
//    QVector3D result(0,0,0);

//    for (int i = 0; i < modelFinal->vertexCount(); i++){
//        double l = currentStep.z(),
//              x = modelOriginal->GetVertex(i).x(),
//              y = modelOriginal->GetVertex(i).y();
//        double ls = cos(l) * x - sin(l)*y + currentStep.x() - modelFinal->GetVertex(i).x(),
//              rs = sin(l) * x + cos(l)*y + currentStep.y() - modelFinal->GetVertex(i).y();
//        QVector3D add = QVector3D(
//                    2 * (ls * 1 + rs * 0 ),
//                    2 * (-ls * 0 + rs * 1 ),
//                    2 * (ls * (-x * sin(l) - y * cos(l)) + rs * (cos(l) * x - sin(l) * y) )
//                    );
//        result = QVector3D(result.x() + add.x(), result.y() + add.y(), result.z() + add.z());
//    }


//    QVector<Derivable> dd = AutoDiffThisShit(modelOriginal, modelFinal, currentStep);

//    result = QVector3D(dd[0].getProiz(), dd[1].getProiz(), dd[2].getProiz());
//    qDebug() << result;

//    return result;
//}

//QVector3D Descent::CurrentGradientDistValue() const{
//    double epsilon = .00001;
//    QVector3D result;
//    for (int i = 0; i < 3; i++){
//        double d0 = DistValue(TranslateAndRotate(modelOriginal,
//                                                QVector3D(currentStep.x(),
//                                                          currentStep.y(),
//                                                          currentStep.z()))),
//              d1 = DistValue(TranslateAndRotate(modelOriginal,
//                                                 QVector3D(currentStep.x() + epsilon * (i==0),
//                                                           currentStep.y() + epsilon * (i==1),
//                                                           currentStep.z() + epsilon * (i==2))));
//        result[i] = (d1 - d0) / epsilon;
//    }
//    //result[2] = 555;
//    //qDebug() << "Proizvod: " << result;
//    return result;
//}

//double Descent::Module(QVector3D qv) const
//{
//    return sqrt(qv.x() * qv.x() + qv.y() * qv.y() + qv.z() * qv.z());
//}



