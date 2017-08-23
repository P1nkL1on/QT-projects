#include "graphicsobject.h"

GraphicsObjectStruct::GraphicsObject::GraphicsObject()
{

}

QVector3D *GraphicsObjectStruct::GraphicsObject::RayIntersection(const QVector3D *rayStart, const QVector3D *rayFinish, const unsigned int polygonIndex) const
{
    return NULL;
}

QVector<QVector3D> GraphicsObjectStruct::GraphicsObject::GetVertexes(unsigned int polygonIndex) const
{
    return {};
}

QVector<QVector3D> GraphicsObjectStruct::GraphicsObject::GetVertexNormals(unsigned int polygonIndex) const
{
    return {};
}

//QString GraphicsObjectStruct::GraphicsObject::DrawItSelf(QVector<QVector2D> &points, QMatrix4x4 view, QMatrix4x4 perspective)
//{
//    // do nothing with points
//    return "Object is NULL graphics object";
//}
