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
