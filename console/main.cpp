#include <QCoreApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();
    char c;

    for (int i =0; i< 255; i++)
        std::cout << (char)i << " ";


    return 0;

}
