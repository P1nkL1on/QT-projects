#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H
#include "QVector"
#include "QMatrix4x4"
#include "QPainter"
namespace GraphicsObjectStruct  {

class GraphicsObject
{
public:
    GraphicsObject();
    virtual QString ApplyDrawToCanvas (QPainter* painter,const QMatrix4x4 view, const QMatrix4x4 perspective,
                                    const int width, const int hei) = 0;
};
}
#endif // GRAPHICSOBJECT_H
