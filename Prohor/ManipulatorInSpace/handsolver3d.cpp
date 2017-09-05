#include "handsolver3d.h"

HandSolver3D::HandSolver3D()
{

}

HandSolver3D::HandSolver3D(Manipulator3D *m, QVector3D fp)
{
     finalPoint = fp;
     original = m;
     current = NULL;
}

QString HandSolver3D::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    Manipulator3D::DrawLineToCanvas(finalPoint, finalPoint,
                  painter, view, perspective, width, hei);

      if (original == NULL)
            return "No original hand";
      original->ApplyDrawToCanvas(painter, view, perspective, width, hei);

      if (current == NULL)
            return "No current hand";
      current->ApplyDrawToCanvas(painter, view, perspective, width, hei);

      return QString();
}
