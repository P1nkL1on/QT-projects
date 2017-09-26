#include "modelfbx.h"

ModelFBX::ModelFBX()
{
    modelColor = Qt::green;
    vertexes = {};
    polygon_start = {};
    polygon_vertex_indexes = {};
    clusters = {};
    limbs = {};
    limbAtts = {};
}

QVector2D toScrCoords (const QVector2D point, const int screenWidth, const int screenHeight){
    double size = 1.0;
    return QVector2D (screenWidth/2 + (screenWidth/2 * (point[0])) * size, screenHeight * 3 /4 - (screenHeight/2 * (point[1])) * size);
}


QString ModelFBX::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    // first get a point array
    QVector<QVector2D> resPoints = {};
    QString err = DrawItSelf(resPoints, vertexes, view, perspective);

    if (!err.isEmpty())    // force quit
        return err;

    QPen pen;

    // cluster drawing lol
    for (int i = 0; i < clusters.length(); i++){
        QVector3D middlePoint = {0,0,0};
        for (int j = 0; j < clusters[i].indexes.length(); j++)
            middlePoint = QVector3D(  middlePoint.x() + vertexes[clusters[i].indexes[j]].x()
                                    , middlePoint.y() + vertexes[clusters[i].indexes[j]].y()
                                    , middlePoint.z() + vertexes[clusters[i].indexes[j]].z());
        middlePoint = QVector3D(middlePoint.x() / clusters[i].indexes.length(), middlePoint.y() / clusters[i].indexes.length(), middlePoint.z() / clusters[i].indexes.length());
        QVector<QVector2D> centerClusterPoint = {};
        QString errC = DrawItSelf(centerClusterPoint,{middlePoint}, view, perspective);
        QVector2D res = toScrCoords(centerClusterPoint[0], width, hei);
        pen.setWidth(10);
        pen.setColor(Qt::red);
        painter->setPen(pen);
        painter->drawPoint((int)res[0],(int)res[1]);
        pen.setWidth(1);

        for (int j = 0; j < clusters[i].indexes.length(); j++){
            QVector<QVector2D> controllablePoint = {};
            QString errCC = DrawItSelf(controllablePoint,{vertexes[clusters[i].indexes[j]]}, view, perspective);
            QVector2D controllable2D = toScrCoords(controllablePoint[0], width, hei);
            pen.setColor(QColor(255,0,0,(int)(255.0 * clusters[i].weights[j])));
            painter->setPen(pen);
            painter->drawLine((int)res[0],(int)res[1], (int)controllable2D[0], (int)controllable2D[1]);
        }
    }
    pen.setWidth(2);
    pen.setColor(modelColor);//((currentPolygon == polygonSelectedIndex)? Qt::red : modelColor);
    painter->setPen(pen);

    for (int currentVert = 0; currentVert < vertexes.length(); currentVert ++){
        QVector2D res = toScrCoords(resPoints[currentVert], width, hei);
        painter->drawPoint((int)res[0],(int)res[1]);
    }

    // joint drawing



    pen.setWidth(6);
    pen.setColor(Qt::blue);
    painter->setPen(pen);
    for (int i = 0; i < limbs.length(); i++){
        QVector<QVector2D> resJoint = {};
        QVector3D finalTranform = {limbs[0].translation.x(), limbs[0].translation.y(), limbs[0].translation.z()};
        LimbNode* ln = &limbs[i];
        do {
            finalTranform = {finalTranform.x() + ln->translation.x(),
                             finalTranform.y() + ln->translation.y(),
                             finalTranform.z() + ln->translation.z()};
            ln = ln->pater;
        }while(ln != NULL);

        QString errJ = DrawItSelf(resJoint, {finalTranform}, view, perspective);

        QVector2D res = toScrCoords(resJoint[0], width, hei);
        painter->drawPoint((int)res[0],(int)res[1]);
    }
}

QString ModelFBX::DrawItSelf(QVector<QVector2D> &resultPoints, const QVector<QVector3D> vertGiven,
                          const QMatrix4x4 view, const QMatrix4x4 perspective){

    for (int i = 0; i < vertGiven.length(); i++){
        QVector4D vertexCoords = QVector4D(vertGiven[i].x(), vertGiven[i].y(), vertGiven[i].z(), 1.0 ) ;

        vertexCoords = vertexCoords * view * perspective;
        resultPoints << QVector2D (vertexCoords[0] / vertexCoords[3], vertexCoords[1] / vertexCoords[3]);
    }
    return QString();
}
