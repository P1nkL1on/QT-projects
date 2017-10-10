#ifndef MODELFBX_H
#define MODELFBX_H
#include "QVector";
#include "graphicsobject.h"
#include "QImage"
#include "qvector2d.h"
#include "structsFBX.h"

class ModelFBX : public GraphicsObjectStruct::GraphicsObject
{
private:

    QString DrawItSelf(QVector<QVector2D> &points,const QVector<QVector3D> vertGiven,
                       const QMatrix4x4 view, const QMatrix4x4 perspective);

public:
    QColor modelColor;
    ModelFBX();
    QVector<QVector3D> vertexes;
    QVector<unsigned int> polygon_vertex_indexes;
    QVector<unsigned int> polygon_start;
    QVector<Cluster> clusters;
    QVector<LimbNode> limbs;
    QVector<LimbNodeAttribute> limbAtts;
    QVector<AnimCurve> animCurves;
    QVector<AnimNode> animNodes;

    QString ApplyDrawToCanvas(QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                           const int width, const int hei) override;
};

#endif // MODELFBX_H
