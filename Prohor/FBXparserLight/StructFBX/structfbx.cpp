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
    // (r1 * r2 * r3) * V
    // ротаёшоны применяются в порядке справа налево - 3 2 1

    // QMatrix M; M.Rotate(); M.Transform(); === M * R * T; -- обратынй поряок


    QMatrix4x4 rotateMatrix;

    rotateMatrix.rotate(Rotation.x(), 1.0, 0, 0);
    rotateMatrix.rotate(Rotation.y(), 0, 1.0, 0);
    rotateMatrix.rotate(Rotation.z(), 0, 0, 1.0);

    QVector4D temp = rotateMatrix * QVector4D(Transform, 1.0);

    return QVector3D(to.x() + temp.x(), to.y() + temp.y(), to.z() + temp.z());

}

QVector3D CommonFuncs::AddDirectWtParent(const QVector3D to, const QVector3D Transform, const QVector3D wasRotation, const QVector3D addRotation)
{
    QMatrix4x4 wasRotateMatrix, addRotateMatrix;
    wasRotateMatrix.rotate(wasRotation.x(), 1.0, 0, 0);
    wasRotateMatrix.rotate(wasRotation.y(), 0, 1.0, 0);
    wasRotateMatrix.rotate(wasRotation.z(), 0, 0, 1.0);

    addRotateMatrix.rotate(addRotation.x(), 1.0, 0, 0);
    addRotateMatrix.rotate(addRotation.y(), 0, 1.0, 0);
    addRotateMatrix.rotate(addRotation.z(), 0, 0, 1.0);

    QVector4D temp = addRotateMatrix * wasRotateMatrix * QVector4D(Transform, 1.0);

    return QVector3D(to.x() + temp.x(), to.y() + temp.y(), to.z() + temp.z());
}
