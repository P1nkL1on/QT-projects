#ifndef TESTVIEWER_H
#define TESTVIEWER_H
#include "QVector"
#include "QVector2D"
#include "camera.h"
#include <QMainWindow>
#include "QPainter"

using namespace  GraphicsObjectStruct;


class TestViewer
{
private:
    QVector<GraphicsObject*> objectsInScene;
public:
    TestViewer();
    void addGraphicsObject (GraphicsObject* newObj);
    void drawOn (QPainter* painter, const Camera cam, const int wid, const int hei) const;

};

#endif // TESTVIEWER_H
