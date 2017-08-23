#include "testviewer.h"
#include "iostream"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qvector3d.h"
TestViewer::TestViewer()
{
    objectsInScene = {};
    lightsInScene = {};
}


void TestViewer::addGraphicsObject(GraphicsObject *newObj)
{
   objectsInScene << newObj;
}
void TestViewer::addLight(LightSourse *light)
{
    lightsInScene << light;
}

void TestViewer::drawOn(QPainter *painter, const Camera cam, const int wid, const int hei) const{
    if (objectsInScene.length() == 0)
        qDebug() << "No objects to rasterise.";

    for (int currentModel = 0; currentModel < objectsInScene.length(); currentModel++)
        objectsInScene[currentModel]->ApplyDrawToCanvas(painter, cam.getViewingMatrix(), cam.getPerspectiveMatrix(), wid, hei);

}

GraphicsObject *TestViewer::getModel() const
{
    if (objectsInScene.length() == 0)
        return NULL;
    return objectsInScene[0];
}

QVector<LightSourse *> TestViewer::getLights() const
{
    if (lightsInScene.length() == 0)
        return {};
    return lightsInScene;
}

