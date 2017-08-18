#ifndef CAMERA_H
#define CAMERA_H
#include "modelloader.h"
#include "qmatrix4x4"
#include "qvector"
#include "QVector2D"
using namespace ModelLoader;

class Camera
{
private:
    QVector<QVector3D> cameraProps;
    double fieldAngle;
    double fieldDistance;
    double focusDistance;

    QMatrix4x4 perspectiveMatrix;
    QMatrix4x4 viewingMatrix;
    void calculateMatrixes(bool perspective, bool viewing);
    void move ( const QVector3D addCoordinates );
    void rotate (  const float angle, const QVector3D os );
    void scale(  const double scale );
public:
    QVector2D prevMousePlace;
    Camera( double fAngle, double fDist, double focusDist );
    QVector3D cameraCenter() const;
    QVector3D cameraDirection() const;

    QMatrix4x4 getPerspectiveMatrix() const;
    QMatrix4x4 getViewingMatrix() const;

    void transformByMouseMoving (const QVector2D currentMousePlace,
                                 const unsigned short actionType );
};

#endif // CAMERA_H
