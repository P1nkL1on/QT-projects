#include "color.h"
#include <QApplication>

// standart
Color::Color()
{
    this->red = 0;
    this->green = 0;
    this->blue = 0;
}

Color::Color (int clr){
    clr = std::max(0, std::min(255, clr));
    this->red = clr; this->blue = clr; this->green = clr;
}

// double color
Color::Color(double R, double G, double B)
{
    this->red = std::max(0.0, std::min(255.0, R));
    this->green = std::max(0.0, std::min(255.0, G));
    this->blue = std::max(0.0, std::min(255.0, B));
}

// int color typical
Color::Color(int R, int G, int B)
{
    this->red = std::max(0, std::min(255, R));
    this->green = std::max(0, std::min(255, G));
    this->blue = std::max(0, std::min(255, B));
}

//Color Color::operator + (const Color& clr) const{
//    return Color( red + clr.red, green + clr.green, blue + clr.blue );
//}
//Color Color::operator *(const double k) const{
//    return Color (red * k, green * k, blue * k);
//}

double Color::R (){
    return red;
}
double Color::G() {
    return green;
}
double Color::B (){
    return blue;
}
