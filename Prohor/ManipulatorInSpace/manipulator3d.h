#ifndef MANIPULATOR3D_H
#define MANIPULATOR3D_H
#include "graphicsobject.h"
#include "qvector3d.h"
#include "Eigen/Core"

class Manipulator3D : public GraphicsObjectStruct::GraphicsObject
{
private:
    QVector<float> armLengths;
    QVector<QVector3D> armAngles;
    QVector3D startPoint;
public:
    QColor selfColor;
    Manipulator3D();
    Manipulator3D(QVector3D startPnt, QVector<float> lengthes);
    Manipulator3D(QVector3D startPnt, QVector<float> lengthes, QVector<QVector3D> angles);

    QVector3D GetStartPoint() const;
    int segmentCount() const;
    Manipulator3D *Copy(QVector<QVector3D> newAngles) const;
    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
    static QString DrawLineToCanvas ( const QVector3D p1, const QVector3D p2,
                                      QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                                      const int width, const int hei);
    QVector3D pointWithOffsetAngle (const QVector3D point0, const float dist, const QVector3D angles);
    template <typename T>
    void GetDistsAndAngles ( QVector<T>& Dists, QVector<T>& AnglesX,
                             QVector<T>& AnglesY, QVector<T>& AnglesZ,
                             const QVector<QVector3D> transform){
        AnglesX = {};
        AnglesY = {};
        AnglesZ = {};
        Dists = {};
        for (int i = 0; i < armLengths.length(); i++){
            Dists << T(armLengths[i]);
            AnglesX << T(armAngles[i].x() + transform[i].x());
            AnglesY << T(armAngles[i].y() + transform[i].y());
            AnglesZ << T(armAngles[i].z() + transform[i].z());
        }
        //__ end
    }
};

#endif // MANIPULATOR3D_H
