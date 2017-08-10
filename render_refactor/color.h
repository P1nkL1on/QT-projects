#ifndef COLOR_H
#define COLOR_H
#include <QApplication>

struct Color
{
private:
    double red;
    double blue;
    double green;
public:
    Color ();
    Color (int R, int G, int B);
    Color (int clr);
    Color (double R, double G, double B);
    Color operator + (const Color& clr) const;
    Color operator * (const double k) const;
    int R();
    int G();
    int B();
};

#endif // COLOR_H
