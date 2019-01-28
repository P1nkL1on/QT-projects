#include "field.h"


Field::Field(const int X, const int Y, const int vero):
    m_x(X),
    m_y(Y)
{
    m_points = QVector<QVector<FieldPoint *>>(Y);
    for (int j = 0; j < Y; ++j){
        m_points[j] = QVector<FieldPoint *>(X);
        for (int i = 0; i < X; ++i){
            m_points[j][i] = new FieldMosh((qrand() % vero == 0)? 100 : 0);
            if (!(qrand() %(X)))
                m_points[j][i]->setCrType("Biter");
        }
    }
}

void Field::draw(QPainter *qp, const int scale)
{
    const int xOff = 20; const int yOff = 20;
    for (int j = 0; j < y(); ++j)
        for (int i = 0; i < x(); ++i){
            QPen pen(m_points[j][i]->color(), scale);
            qp->setPen(pen);
            qp->drawPoint(xOff + i * scale, yOff + j * scale);
        }
}

int Field::y() const
{
    return m_y;
}

int Field::x() const
{
    return m_x;
}

QVector<int> Field::neightIndexes(const int xo, const int yo) const
{
    QVector<int> res;
    if (xo > 0 && yo > 0)res << xo-1 << yo-1;
    if (xo > 0) res << xo-1 << yo;
    if (yo > 0) res << xo << yo - 1;
    if (xo < x() - 1 && yo < y() - 1) res << xo+1 << yo+1;
    if (xo < x() -1) res << xo+1 << yo;
    if (yo < y() -1) res << xo << yo +1;
    if (yo < y() -1 && xo > 0) res << xo - 1 << yo +1;
    if (xo < x() - 1 && yo > 0) res << xo+ 1 << yo-1;
    return res;
}

QVector<FieldPoint *> Field::neights(const int x, const int y) const
{
    QVector<FieldPoint *> res;
    QVector<int> inds = neightIndexes(x,y);
    for (int i = 0; i < inds.length(); i += 2)
        res << m_points[inds[i + 1]][inds[i]];
    return res;
}

void Field::step(){
    for (int j = 0; j < y(); ++j)
        for (int i = 0; i < x(); ++i)
            if (m_points[j][i]->type() != "Empty")
                m_points[j][i]->think(neights(i,j));
    for (int j = 0; j < y(); ++j)
        for (int i = 0; i < x(); ++i)
            if (m_points[j][i]->type() != "Empty")
                m_points[j][i]->step();
}
