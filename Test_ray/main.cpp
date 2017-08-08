#include <QCoreApplication>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

using std::cin;
using std::endl;
using std::cout;

/*int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}*/

struct Color{
    double R;
    double G;
    double B;
    Color () { R = 0; G = 0; B = 0;}
    Color (double r, double g, double b){ R = r; G = g; B = b;}

};

int main (){

    FILE* file = fopen("out.bmp","w");

    const unsigned short screen_width = 60;
    const unsigned short screen_height = 40;
    Color pixel_matrix [screen_width][screen_height];

    for (int i = 0; i < screen_width; i++)
        for (int j=0; j < screen_height; j++)
        {
            (pixel_matrix[i][j]) = Color(20,200,50);
            fwrite(&(pixel_matrix[0][0]), i, j, file);
        }


    return 0;
}
