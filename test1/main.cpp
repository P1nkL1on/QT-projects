#include <QCoreApplication>
#include <iostream>

using std::cout;
using std::endl;
using std:: cin;


bool input_int (int *a){
    cout << "Введите число a:" << endl;
    if (cin >> (*a))
        return true;
    else
        return false;
}

void swap (int& a, int& b){
    int temp = a;
    a = b; b = temp;
}

int main(int argc, char *argv[])
{

    setlocale(LC_ALL, "Russian");
/*
    int x = 20;
    int y = 20;

    float A[100];
    A[0] = 1;
    A[1] = 2;
    //QCoreApplication a(argc, argv);
    cout << A[0] << "/" << A[1] << endl;

    cout << &x << "/" << &y << endl;

    cout << "Введите длину динам. массива" << endl;
    int N;
    cin >> N;

    int *p = new int[N];
    // выделение памяти з кучи для хранения массиа по указателю p
    p[0] = 1; p[1] = 2; // назначение элементов
    *(p+2) = 3; *(p+3) = 4;
    for (int i(0); i<N; i++)
           *(p + i) = i * (-2);
    //cout << *p << '/' << *(p+1) <<endl;
    for (int i =0; i<N; i++)
            cout << "p[" << i << "] = " << *(p+i) << endl;
    delete [] p;*/
int b = 0;
input_int(&b);
cout  << "   b = " << b << endl;

int c = 0;
int &ssilka = c;

int d = 10; int e = 15;
cout << d << "/" << e << endl;
swap(d,e);
cout << d << "/" << e << endl;

cout << "Введите M N" << endl;


    int N;
    int M;
    cin >> M >> N;
    int** p =  new int*[N];
    for (int i=0; i< N; i++)
        p[i] = new int[M];


    for (int i =0; i<M; i++){
       for (int j =0; j< N; j++)
       {
            *(*(p+i)+j) = i + j;
            cout << "\t"<<p[i][j]<<" ";

       }
       cout << endl;
    }


    // delete quad
    for (int i=0; i<N; i++)
            delete [] p[i];
    // delete total pointer
    delete p;



    //return a.exec();
    cout << "Hello, world!";//<<endl<<"Second stroke"<<endl<<"Rofel\n\n\n\nRofel";
    return 0;
}
