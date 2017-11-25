#include "structfbx.h"




Joint::Joint()
{
    ID = QString();
    name = QString();
    pater = NULL;
    kids = {};
    localTranslation = QVector3D();
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
    bindMatrix = QMatrix4x4();
    bindTransform = QVector3D();
}

Joint::Joint(QString ID0, QString name0)
{
    this->ID = ID0;
    this->name = name0;
    pater = NULL;
    kids = {};
    localTranslation = QVector3D();
    currentTranslation = QVector3D();
    currentRotation = QVector3D();
    bindMatrix = QMatrix4x4();
    bindTransform = QVector3D();
}


AttendedVertex::AttendedVertex()
{
    jointIndexs = {};
    weights = {};
    localJointCoords = {};
}

QVector3D CommonFuncs::AddDirect(const QVector3D to, const QVector3D Transform, const QVector3D Rotation)
{
    QMatrix4x4 rotateMatrix;

    rotateMatrix.rotate(Rotation.x(), 1.0, 0, 0);
    rotateMatrix.rotate(Rotation.y(), 0, 1.0, 0);
    rotateMatrix.rotate(Rotation.z(), 0, 0, 1.0);

    QVector4D temp = QVector4D(Transform, 1.0) * rotateMatrix;

    return QVector3D(to.x() + temp.x(), to.y() + temp.y(), to.z() + temp.z());

}
