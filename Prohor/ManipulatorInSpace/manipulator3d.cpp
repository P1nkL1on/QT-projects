#include "manipulator3d.h"
#include "qvector2d.h"
#include "qvector4d.h"

#include"qmath.h"

using namespace Eigen;

Manipulator3D::Manipulator3D()
{

}

Manipulator3D::Manipulator3D(QVector3D startPnt, QVector<float> lengthes)
{
    armLengths = lengthes;
    startPoint = startPnt;
    armAngles = QVector<QVector3D>();
    for (int i = 0; i < armLengths.length(); i++)
        armAngles << QVector3D(0.3,0,0);
}

Manipulator3D::Manipulator3D(QVector3D startPnt, QVector<float> lengthes, QVector<QVector3D> angles)
{
    armAngles = angles;
    armLengths = lengthes;
    startPoint = startPnt;
}

QVector2D ToScreenCoordinates (const QVector3D point, const QMatrix4x4 view,
                               const QMatrix4x4 perspective, const int screenWidth, const int screenHeight){
    double size = 1.0;
    QVector4D addPointCoords(point.x(), point.y(), point.z(), 1);
    addPointCoords = addPointCoords * view * perspective;
    return QVector2D (screenWidth/2 + (screenWidth/2 * (addPointCoords[0] / addPointCoords[3])) * size,
                      screenHeight * 3 /4 - (screenHeight/2 * (addPointCoords[1] / addPointCoords[3])) * size);
}

QString Manipulator3D::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view,
                                         const QMatrix4x4 perspective, const int width, const int hei)
{
    QVector3D currentAngle(0,0,0);
    QVector3D currentPoint(startPoint.x(), startPoint.y(), startPoint.z());
    QVector2D current2DPoint = ToScreenCoordinates(startPoint, view, perspective, width, hei);

    for (int i = 0; i < armLengths.length(); i++){
        currentAngle = QVector3D(armAngles[i].x() + currentAngle.x(),
                                 armAngles[i].y() + currentAngle.y(),
                                 armAngles[i].z() + currentAngle.z());
        // calculate a coordinate of pont number 0
        QVector3D newPoint;//(currentPoint.x() + armLengths[i], currentPoint.y(), currentPoint.z());
        newPoint = pointWithOffsetAngle(currentPoint, armLengths[i], currentAngle);

        QVector2D new2DPoint = ToScreenCoordinates(newPoint, view, perspective, width, hei);
        // drawing current segment
        QPen p;
        p.setColor(selfColor);
        p.setWidth(3);
        painter->setPen(p);
        painter->drawPoint((int)current2DPoint.x(), (int)current2DPoint.y());
        p.setWidth(1);
        painter->setPen(p);
        painter->drawLine((int)current2DPoint.x(), (int)current2DPoint.y(),
                          (int)new2DPoint.x(), (int)new2DPoint.y());
        // redeclaration
        currentPoint = newPoint;
        current2DPoint = new2DPoint;
    }
    return QString();
}

QString Manipulator3D::DrawLineToCanvas(const QVector3D p1, const QVector3D p2,
                                        QPainter *painter, const QMatrix4x4 view,
                                        const QMatrix4x4 perspective, const int width, const int hei)
{
    QVector2D p12D = ToScreenCoordinates(p1, view, perspective, width, hei);
    QVector2D p22D = ToScreenCoordinates(p2, view, perspective, width, hei);
    // drawing current segment
    QPen p;
    p.setColor(Qt::red);
    p.setWidth(1);
    painter->setPen(p);
    painter->drawLine((int)p12D.x(), (int)p12D.y(), (int)p22D.x(), (int)p22D.y());

    return QString();
}

QVector3D Manipulator3D::pointWithOffsetAngle(const QVector3D point0, const float dist,
                                              const QVector3D angles)
{
     Matrix<double, 3, 3> rotat(3,3);
     rotat(0,0) = cos(angles.x())*cos(angles.z()) - sin(angles.x())*cos(angles.y())*sin(angles.z());
     rotat(0,1) = -cos(angles.x())*sin(angles.z()) - sin(angles.x())*cos(angles.y())*cos(angles.z());
     rotat(0,2) = sin(angles.x())*sin(angles.y());
     rotat(1,0) = sin(angles.x())*cos(angles.z()) + cos(angles.x())*cos(angles.y())*sin(angles.z());
     rotat(1,1) = -sin(angles.x())*sin(angles.z()) + cos(angles.x())*cos(angles.y())*cos(angles.z());
     rotat(1,2) = -cos(angles.x())*sin(angles.y());
     rotat(2,0) = sin(angles.y()) * sin(angles.z());
     rotat(2,1) = sin(angles.y())*cos(angles.z());
     rotat(2,2) = cos(angles.y());
     Matrix<double, 3, 1> leng(3,1);
     leng(0,0) = dist; leng(1,0) = 0; leng(2,0) = 0;
     Matrix<double,3,1> original(3,1);
     original(0,0) = point0.x();
     original(1,0) = point0.y();
     original(2,0) = point0.z();

     original = rotat * leng + original;
     return QVector3D(original(0,0), original(1,0), original(2,0));
}
