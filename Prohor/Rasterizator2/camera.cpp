#include "camera.h"
#include "modelloader.h"
#include "QMatrix4x4"
#include "QVector4D"
using namespace ModelLoader;


Camera::Camera(double fAngle, double fDist, double focusDist)
{
    // make a props
    cameraProps = { Vertex(-10, 0, 0), Vertex(0,0,0), Vertex(-10, 1, 0), Vertex(-10, 0, -1) };
    // make a focus props
    fieldAngle = fAngle / 180.0 * M_PI;
    fieldDistance = fDist;
    focusDistance = focusDist;
    calculateMatrixes(true, true);
}

void Camera::calculateMatrixes(bool perspective, bool viewing)
{
    Vertex up(cameraProps[2].X - cameraProps[0].X, cameraProps[2].Y - cameraProps[0].Y, cameraProps[2].Z - cameraProps[0].Z);
    Vertex left(cameraProps[3].X - cameraProps[0].X, cameraProps[3].Y - cameraProps[0].Y, cameraProps[3].Z - cameraProps[0].Z);
    Vertex forward( cameraProps[0].X - cameraProps[1].X, cameraProps[0].Y - cameraProps[1].Y, cameraProps[0].Z - cameraProps[1].Z );
    // creating a view matrix

    if (viewing)

        viewingMatrix = QMatrix4x4 ( left.X, up.X, forward.X, 0,
                                    left.Y, up.Y, forward.Y, 0,
                                    left.Z, up.Z, forward.Z, 0,
                                    -left.X * cameraProps[0].X - left.Y * cameraProps[0].Y - left.Z * cameraProps[0].Z,
                                    -up.X * cameraProps[0].X - up.Y * cameraProps[0].Y - up.Z * cameraProps[0].Z,
                                    -forward.X * cameraProps[0].X - forward.Y * cameraProps[0].Y - forward.Z * cameraProps[0].Z, 1);
    //

    if (!perspective) return;

    perspectiveMatrix = QMatrix4x4();
    //perspectiveMatrix.frustum(-10, 10, -10, 10, 20, 200);
    perspectiveMatrix.frustum(-10, 10, -10, 10, 2 * focusDistance / cos (fieldAngle), 2 * fieldDistance / cos (fieldAngle));


}
void Camera::Rotate (const float angle, const QVector3D os ){
    QMatrix4x4 rot;
    rot.rotate(angle, os);
    for (int i = 0; i < cameraProps.length(); i++){
        QVector4D coords = QVector4D( cameraProps[i].X, cameraProps[i].Y, cameraProps[i].Z, 1.0 );
        coords = coords * rot;
        cameraProps[i] = Vertex(coords[0] / coords[3], coords[1] / coords[3], coords[2] / coords[3]);
    }
}

Vertex Camera::cameraCenter()
{
    return cameraProps[0];
}

Vertex Camera::cameraDirection()
{
    return cameraProps[1];
}
