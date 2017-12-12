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

    QMatrix4x4 localTransformMatrix;
    QMatrix4x4 globalTransformMatrix;

    Joint();
    Joint(QString ID0, QString name0);

    void RecaulculateLocalTransformMatrix ();
    void ResetGlobalTransformMatrix();
    void CalculateGlobalTransformMatrix();
};


struct AttendedVertex{
public:
    QVector<int> jointIndexs;
    QVector<float> weights;
    QVector<QVector3D> localJointCoords;
    AttendedVertex();
};

namespace CommonFuncs {
    QVector3D AddDirectMatrx (const QVector3D localTransform, const QMatrix4x4 transform);
    QVector3D AddDirect (const QVector3D to, const QVector3D Transform, const QVector3D Rotation);
    QVector3D AddDirectWtParent (const QVector3D to, const QVector3D Transform, const QVector3D wasRotation, const QVector3D addRotation );
}



#endif // STRUCTFBX_H
