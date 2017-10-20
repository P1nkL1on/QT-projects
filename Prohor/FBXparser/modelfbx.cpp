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
    animCurves = {};
    animNodes = {};
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

    for (int i = 0; i < limbs.length(); i++){
        pen.setColor(Qt::blue);
        pen.setWidth(10);
        painter->setPen(pen);
        QVector<QVector2D> resJoint = {};
        QVector3D finalTranform = {};//{limbs[0].translation.x(), limbs[0].translation.y(), limbs[0].translation.z()};
        LimbNode* ln = &limbs[i];
        //__________
        QVector3D prevTransform = {-ln->translation.x(), -ln->translation.y(), -ln->translation.z()};
        do {

            finalTranform = {finalTranform.x() + ln->translation.x(),
                             finalTranform.y() + ln->translation.y(),
                             finalTranform.z() + ln->translation.z()};
            ln = ln->pater;
        }while(ln != NULL);
        prevTransform = {finalTranform.x() + prevTransform.x(),
                         finalTranform.y() + prevTransform.y(),
                         finalTranform.z() + prevTransform.z()};
//        QVector3D prevTransform;
//        finalTranform = QVector3D(ln->translation.x(), ln->translation.y(), ln->translation.z());
//        if (ln->pater != NULL)
//            prevTransform = QVector3D(ln->pater->translation.x(), ln->pater->translation.y(), ln->pater->translation.z());
//        else
//            prevTransform = finalTranform;
        //___________
        ln = &limbs[i];
        finalTranform = ln->globalTranslation;
        if (ln->pater != NULL)
            prevTransform = ln->pater->globalTranslation;
            //_______
        QString errJ = DrawItSelf(resJoint, {finalTranform, prevTransform}, view, perspective);

        QVector2D res = toScrCoords(resJoint[0], width, hei);
        QVector2D resPater = toScrCoords(resJoint[1], width, hei);
        painter->drawPoint((int)res[0],(int)res[1]);
        pen.setWidth(5);
        painter->setPen(pen);
        painter->drawLine((int)res[0],(int)res[1], (int)resPater[0], (int)resPater[1]);

        painter->drawText((int)res[0],(int)res[1],300,20,0, (limbs[i].name));

        // cluster boys
        pen.setWidth(1);
        pen.setColor(QColor(0,0,255,1));
        painter->setPen(pen);

        for (int j = 0; j < limbs[i].indexes.length(); j++){
            QVector<QVector2D> controllablePoint = {};
            QString errCC = DrawItSelf(controllablePoint,{vertexes[limbs[i].indexes[j]]}, view, perspective);
            QVector2D controllable2D = toScrCoords(controllablePoint[0], width, hei);

            painter->drawLine((int)res[0],(int)res[1], (int)controllable2D[0], (int)controllable2D[1]);
        }
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
