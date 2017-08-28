#include "linegraphics.h"

LineGraphics::LineGraphics()
{

}

void LineGraphics::PushValue(float value)
{
    values << value;
    if (values.length() > 400)
        values.removeFirst();
}

void LineGraphics::DrawItSelf(QPainter *qp, int x0, int y0) const
{
    QPen pe;
    pe.setColor(Qt::black);
    qp->setPen(pe);

    qp->drawLine ( x0, y0 + 100, x0 + 400, y0 + 100 );
    qp->drawLine( x0, y0, x0, y0 + 100);

    pe.setColor(Qt::red);
    qp->setPen(pe);
    for (int i = 0; i < values.length(); i++)
        qp->drawPoint(x0 + 1 + i, y0 + 100 - (int)(values[i]));
}
