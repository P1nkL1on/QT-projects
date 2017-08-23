#include "model.h"
#include <QVector2D>
#include "QVector3D"
#include "iostream"
#include "stereometry.h"

using namespace Stereometry;

namespace ModelStructs{
Model::Model()
{
    modelColor = QColor(Qt::black);
    polygon_start = {0};
}

QString Model::DrawItSelf(QVector<QVector2D> &resultPoints, const QVector<QVector3D> vertGiven,
                          const QMatrix4x4 view, const QMatrix4x4 perspective){

    for (int i = 0; i < vertGiven.length(); i++){
        QVector4D vertexCoords = QVector4D(vertGiven[i].x(), vertGiven[i].y(), vertGiven[i].z(), 1.0 ) ;

        vertexCoords = vertexCoords * view * perspective;
        resultPoints << QVector2D (vertexCoords[0] / vertexCoords[3], vertexCoords[1] / vertexCoords[3]);
    }
    return QString();
}

QVector2D toScrCoords (const QVector2D point, const int screenWidth, const int screenHeight){
    double size = 1.0;
    return QVector2D (screenWidth/2 + (screenWidth/2 * (point[0])) * size, screenHeight * 3 /4 - (screenHeight/2 * (point[1])) * size);
}

QString Model::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective,
                              const int width, const int height){

    // first get a point array
    QVector<QVector2D> resPoints = {};
    QString err = DrawItSelf(resPoints, vertexes, view, perspective);

    QVector<QVector2D> normalPoints = {};
    QString errNormal = DrawItSelf( normalPoints, vertex_normals, view, perspective);

    if (!err.isEmpty())    // force quit
        return err;

    QPen pen;
    pen.setWidth(2);
    pen.setColor(modelColor);//((currentPolygon == polygonSelectedIndex)? Qt::red : modelColor);
    painter->setPen(pen);

    for (int currentPolygon = 0; currentPolygon < polygon_start.length() - 1; currentPolygon++){
        QVector2D lastPointInPolygon = toScrCoords(resPoints[polygon_vertex_indexes[polygon_start[currentPolygon + 1] - 1] - 1], width, height);
        for (int currentVertInd = polygon_start[currentPolygon]; currentVertInd < polygon_start[currentPolygon + 1]; currentVertInd++){

            QVector2D res = toScrCoords(resPoints[polygon_vertex_indexes[currentVertInd] - 1], width, height);
            painter->drawLine((int)res[0],(int)res[1], (int)lastPointInPolygon[0], (int) lastPointInPolygon[1]);
            lastPointInPolygon = res;
        }
    }
    // draw normals
    pen.setColor(Qt::red);
    painter->setPen(pen);
    if (false)
    if (errNormal.isEmpty() && normalPoints.length() > 0)
        for (int currentNormal = 0; currentNormal < vertex_normals.length(); currentNormal++){
            QVector2D point = toScrCoords(resPoints[currentNormal], width, height),
                      pointN = toScrCoords(normalPoints[currentNormal], width, height);
            painter->drawLine((int)point[0],(int)point[1], (int)pointN[0], (int) pointN[1]);
        }
    return QString();
}

double GetValue (const QVector3D* point, const QVector<double> parametric){
    Q_ASSERT(parametric.length() == 4);
    return parametric[0] * point->x() + parametric[1] * point->y() + parametric[2] * point->z() + parametric[3];
}

QVector3D* Model::RayIntersection(const QVector3D *rayStart,
                                   const QVector3D *rayFinish, const unsigned int polygonIndex) const{
    QVector<QVector3D> trianglePoints = {vertexes[-1 + polygon_vertex_indexes[polygonIndex * 3]],
                                        vertexes[-1 + polygon_vertex_indexes[polygonIndex * 3 + 1]],
                                        vertexes[-1 + polygon_vertex_indexes[polygonIndex * 3 + 2]]};
    QVector3D* intersection = NULL;
    //__________
    //qDebug() << polygonIndex << parametric.length();
    double distance = Dist(*rayStart, *rayFinish);
    QVector3D cosPoint = Mult (Resid (*rayFinish, *rayStart), 1.0/distance);

    double
        fromValue = GetValue(rayStart, parametric[polygonIndex]),
        cosValue = GetValue(&cosPoint, parametric[polygonIndex]) - parametric[polygonIndex][3];
    if (cosValue > .00001 || cosValue < -.00001)
    {
        QVector3D fin = Mult(cosPoint, -(fromValue / (double)cosValue));
        intersection = new QVector3D();
        *intersection = Summ(*rayStart, fin);
    }
    //__________
    if (intersection == NULL)
        return NULL;

    // TRUE == ANGLES, FALSE = BALLICENTER
    if (true){

        if (!(Dist(*intersection, *rayStart ) > .0001 && Dist(*rayFinish, *intersection ) < distance + .0001))
            return NULL;
        double angleDoff = M_PI - (
                           Angle(trianglePoints[0], *intersection, trianglePoints[1])
                         + Angle(trianglePoints[1], *intersection, trianglePoints[2])
                         + Angle(trianglePoints[2], *intersection, trianglePoints[0]));
        if (angleDoff > -.001 && angleDoff < .001)
            return intersection;

    }else{
        QVector3D ballic = BallecenterCoordGeron(*intersection, trianglePoints);
        float diff =  1.0 - (ballic[0] + ballic[1] + ballic[2]);
        if (diff > -.000001 && diff < .000001)
            return intersection;
    }
    return NULL;

}
}

