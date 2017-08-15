#ifndef CAMERA_H
#define CAMERA_H
#include "modelloader.h"
#include "qmatrix4x4"
#include "qvector"
using namespace ModelLoader;

class Camera
{
private:
    QVector<Vertex> cameraProps;

public:
    double fieldAngle;
    double fieldDistance;
    double focusDistance;

    Camera( double fAngle, double fDist, double focusDist );
    void calculateMatrixes(bool perspective, bool viewing);
    QMatrix4x4 perspectiveMatrix;
    QMatrix4x4 viewingMatrix;
    Vertex cameraCenter();
    Vertex cameraDirection();
    void Rotate (const float angle, const QVector3D os );
};

#endif // CAMERA_H
