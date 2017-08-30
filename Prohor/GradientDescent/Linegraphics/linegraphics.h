#ifndef LINEGRAPHICS_H
#define LINEGRAPHICS_H

#include "qvector.h"
#include "qpainter.h"

class LineGraphics
{
private:
    QVector<float> values;
public:
    LineGraphics();
    void PushValue ( float value);
    void DrawItSelf (QPainter* qp, int x0, int y0, int wid) const;

};

#endif // LINEGRAPHICS_H
