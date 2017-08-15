#ifndef TESTVIEWER_H
#define TESTVIEWER_H
#include "QVector"
#include "QVector2D"
#include "camera.h"
class TestViewer
{
private:
    double widHeiMult; // коэффицент отношения высоты к ширине (1.5, 2 и т.д.)
public:
    TestViewer();
    QString rasterView(QVector<QVector2D>& resPoints, const Scene scene, const Camera cam );
};

#endif // TESTVIEWER_H
