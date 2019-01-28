#ifndef FIELD_H
#define FIELD_H
#include "fieldpoint.h"
#include <QPainter>

class Field
{
public:
    Field(const int X, const int Y, const int vero);
    void draw(QPainter *qp, const int scale);
    int y() const;
    int x() const;
    void step();
private:
    int m_x;
    int m_y;
    QVector<QVector<FieldPoint *>> m_points;
    QVector<int> neightIndexes(const int x, const int y) const;
    QVector<FieldPoint *> neights(const int x, const int y) const;
};

#endif // FIELD_H
