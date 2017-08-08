#include <QCoreApplication>
#include <iostream>
#include <QFile>
#include <QTextStream>
//#include <QMessageBox>


using std::cout;
using std::endl;
using std:: cin;

struct TPoint{

    double X;
    double Y;
    double Z;

};

struct TTriangle
{
    TPoint *pps[3];
    double parametric[4];
    TPoint center_node;
    TPoint normal_node;
};

void LoadModel (QString path, int& point_count, TPoint all_points[],
                                  int& polygon_count, TTriangle polygons[]){

    //dodecaedr  diamond rabbit
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly)) {
        cout<<"Can't read file"<<endl;
        return;
    }else{
        QTextStream in(&file);
        // select points and polygon memory
         while(!in.atEnd()) {
                // строка из файла
                QString line = in.readLine(); //line[0];
                //cout << line.toStdString()<<"   ::    "<<endl;
                // строку делим по пробелам
                QStringList fields = line.split(" ");


                //cout << (fields.takeAt(0)).toStdString() << endl;
                // первый встречающийся знак (у нужны строк он соответствует нужному)
                QString first = (fields.takeAt(0)).toLower();
                if (first == "v")
                {
                    // массив выловленных координат
                    double coords[3];
                    // предполагается, чтокоординаты будут в непустых кусках
                    // на которые строка разделилась после split(' ')
                    for (int j=0, am = 0; j<fields.count(); j++){
                        // если длина подкуска ненулевая, то значит она содержит число
                        if (fields[j].length() > 0)
                            //  а значит это число мы пихаем в массив найденных координат
                            coords[am++] = ::atof(fields[j].toStdString().c_str());
                        //                                      ^ какой то автопарсер
                    }
                    // присваиваем
                    all_points[point_count++] = {coords[0],coords[1],coords[2]};

                    // трас в консоль, что ты получил (для наглядности)
                    cout << "vertex get ("<<point_count<<
                            ") :: X: "<<all_points[point_count-1].X<<
                                " Y: "<<all_points[point_count-1].Y <<
                                " Z: "<<all_points[point_count-1].Z << endl;
                }
                if (first == "f"){
                    cout<<"Polygon catched "<<fields.count()<<endl;
                    unsigned short at_p = -1;
                    TTriangle tt = {};
                    for (int i = 0; i<fields.count(); i++)
                    {
                        // куски поля
                        std::string pol = fields.at(i).toStdString();
                        if (pol.length() > 0)
                        {
                            // первые 3 конвертируются в новый полигон
                            at_p++;
                            if (at_p < 3){
                                int num = ::atof((pol.substr(0,pol.find('/'))).c_str()) - 1;
                                tt.pps[at_p] = &(all_points[num]);
                                cout << "get to ("<<at_p<<"): "<<tt.pps[at_p]<<"--- "<<sizeof(tt.pps[at_p])<<" bytes"<<endl;
                            }else{
                                if (at_p == 3){
                                    // случай 4х угольного полигона
                                    int num2 = ::atof((pol.substr(0,pol.find('/'))).c_str()) - 1;
                                    // нумер4 ой точки 1 2 3 4 -> 1 2 3,3 4 1
                                    // доабавляем новообразование в полигонную кучу
                                    TTriangle tt2 = {tt.pps[0], tt.pps[2], &(all_points[num2])};
                                    polygons[polygon_count++] = tt2;
                                    // что может пойти не так?
                                    cout << "sqare polygon triangled" << endl;
                                }else{
                                    break;  // пятиугольники и прочее скипаем
                                }

                            }
                        }
                        //if (fields.count>0 && ((fields.takeAt(i)).count()) > 0)
                        //    fields.takeAt(i) = ((fields.takeAt(i)).split("/")).takeAt(0);
                        //cout<<fields.at(i).toStdString().length()<<endl;
                    }
                    polygons[polygon_count++] = tt;
                }
    }

        file.close();
    }
}

//using namespace std;
int main(int argc, char *argv[])
{
    int point_count = 0;
    TPoint all_points[1000];

    int polygon_count = 0;
    TTriangle polygons[2000];


    LoadModel("D:/QT projects/!models/diamond.txt",
              point_count, all_points, polygon_count, polygons);


    std::cout<<"Point count: "<<point_count << "  Polygon count:"<<polygon_count<< endl;
    std::cout<<"hello, world";
    delete all_points;
    return 0;
}
