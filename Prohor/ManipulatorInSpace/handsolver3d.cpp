#include "handsolver3d.h"
#include "QDebug"

HandSolver3D::HandSolver3D()
{

}

HandSolver3D::HandSolver3D(Manipulator3D *m, QVector3D fp)
{
     previousDistance = -1;
     finalPoints = {QPair<int,QVector3D>(m->segmentCount() - 1, fp )};
     original = m;
     current = NULL;
     currentTransofrm = {};
     for (int i = 0; i < original->segmentCount(); i++)
          currentTransofrm << QVector3D(0,0,0);
     original->selfColor = Qt::black;
     //current->selfColor = Qt::green;
}

HandSolver3D::HandSolver3D(Manipulator3D* m, QVector<QPair<int, QVector3D>> fp)
{
    previousDistance = -1;
    finalPoints = fp;
    original = m;
    current = NULL;
    currentTransofrm = {};
    for (int i = 0; i < original->segmentCount(); i++)
        currentTransofrm << QVector3D(0,0,0);
    original->selfColor = Qt::black;
}

QString HandSolver3D::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    for (int i = 0; i < finalPoints.length(); i++)
        Manipulator3D::DrawLineToCanvas(finalPoints[i].second, finalPoints[i].second,
                      painter, view, perspective, width, hei);

      if (original == NULL)
            return "No original hand";
      original->ApplyDrawToCanvas(painter, view, perspective, width, hei);

      if (current == NULL)
            return "No current hand";
      current->ApplyDrawToCanvas(painter, view, perspective, width, hei);

      return QString();
}


/*static T CurrentDist ( const QVector3D targetPoint, const QVector3D startPoint,
                           const QVector<T> dists, const QVector<T> anglesx,
                           const QVector<T> anglesy, const QVector<T> anglesz )*/

void HandSolver3D::Step()
{
    QVector<Derivable> anglesx,  anglesy, anglesz, dists;
    QVector<QVector3D> implementChange = {};

    original->GetDistsAndAngles(dists,anglesx, anglesy, anglesz, currentTransofrm);
    // now we have derivable/float/dist
    for (int i = 0; i < dists.length(); i++){
        QVector<float> changeAngles = {};
        for (int j = 0; j < 3; j++){
            switch (j){
                case 0:
                    anglesx[i] = Derivable(anglesx[i].getValue(), 1.0); break;
                case 1:
                    anglesy[i] = Derivable(anglesy[i].getValue(), 1.0); break;
                case 2:
                    anglesz[i] = Derivable(anglesz[i].getValue(), 1.0); break;
                default:
                    break;
            }
            Derivable distance = CurrentDist(finalPoints, original->GetStartPoint(),
                                             dists, anglesx, anglesy, anglesz);
            //
            //
            if (i + j == 0){
                if (previousDistance != -1){
                    if (distance.getValue() < previousDistance) step *= 1.0007; else step /= 1.01;
                }
                previousDistance = distance.getValue();
                qDebug() << previousDistance;
            }
            changeAngles << distance.getProiz();
            //qDebug () << i << j << distance.getValue() << "/" << distance.getProiz();
        }
        // we have a new increasement
        Q_ASSERT(changeAngles.length() == 3);
        implementChange << QVector3D( changeAngles[0], changeAngles[1], changeAngles[2] );

    }
    // add an implement to main
    Q_ASSERT(currentTransofrm.length() == implementChange.length());
    for (int i = 0; i < currentTransofrm.length(); i++)
        currentTransofrm[i] = QVector3D(currentTransofrm[i].x() - implementChange[i].x() * step  * (i+5) / currentTransofrm.length(),
                                        currentTransofrm[i].y() - implementChange[i].y() * step  * (i+5) / currentTransofrm.length(),
                                        currentTransofrm[i].z() - implementChange[i].z() * step  * (i+5) / currentTransofrm.length());
    current = original->Copy(currentTransofrm);
    current->selfColor = Qt::green;
}
