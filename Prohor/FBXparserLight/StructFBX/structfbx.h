#ifndef STRUCTFBX_H
#define STRUCTFBX_H

#include "qstring.h"
#include "qvector3d.h"
#include "qvector.h"
#include "QMatrix4x4"

struct Joint{
public:
    QString ID;
    QString name;
    Joint* pater;
    QVector<Joint*> kids;
    QVector3D localTranslation;
    QVector3D currentTranslation;
    QVector3D currentRotation;
    QMatrix4x4 bindMatrix;
    QVector3D bindTransform;

    Joint();
    Joint(QString ID0, QString name0);
};


struct AttendedVertex{
public:
    QVector<int> jointIndexs;
    QVector<float> weights;
    QVector<QVector3D> localJointCoords;
    AttendedVertex();
};

namespace CommonFuncs {
    QVector3D AddDirect (const QVector3D to, const QVector3D Transform, const QVector3D Rotation);
    QVector3D AddDirectWtParent (const QVector3D to, const QVector3D Transform, const QVector3D wasRotation, const QVector3D addRotation );
}



#endif // STRUCTFBX_H
