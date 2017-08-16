#include "testviewer.h"
#include "iostream"

#include "mainwindow.h"
#include "ui_mainwindow.h"

TestViewer::TestViewer()
{
    previousResult = {};
}

QString TestViewer::rasterView(QVector<QVector2D>& resultPoints, const Scene scene, const Camera cam ){
    // corners add
    resultPoints = {};//{QVector2D(-1.0, -1.0), QVector2D(-1.0, 1.0), QVector2D(1.0, 1.0), QVector2D(1.0, -1.0)};
    // all other points
    double maxCrd = -100000, minCrd = 100000;

    for (int i = 0; i < scene.vertexes.length(); i++){
        QVector4D vertexCoords = QVector4D(scene.vertexes[i].X, scene.vertexes[i].Y, scene.vertexes[i].Z, 1.0 ) ;
        //std::cout << vertexCoords[0] << " " << vertexCoords[1] << " " << vertexCoords[2] << std::endl;

        vertexCoords = vertexCoords * cam.viewingMatrix;// * cam.perspectiveMatrix;

        QVector2D newPoint = QVector2D (vertexCoords[0] / vertexCoords[3], vertexCoords[1] / vertexCoords[3]);

        if (maxCrd < newPoint[0]) maxCrd = newPoint[0]; if (maxCrd < newPoint[1]) maxCrd = newPoint[1];
        if (minCrd > newPoint[0]) minCrd = newPoint[0]; if (minCrd > newPoint[1]) minCrd = newPoint[1];

        resultPoints << newPoint;
    }
    // if everything is normal
    // then normilise all points to -1 to 1
    //std::cout << "Minimal " << minCrd << "  Maximum " << maxCrd << std::endl;
    for (int i = 0; i < resultPoints.length(); i++){
        resultPoints[i] = QVector2D((resultPoints[i][0] - minCrd) / (maxCrd - minCrd) * 2.0 - 1.0,
                                    (resultPoints[i][1] - minCrd) / (maxCrd - minCrd) * 2.0 - 1.0);
    }
    // save the result
    previousResult = resultPoints;

    return QString();
}


