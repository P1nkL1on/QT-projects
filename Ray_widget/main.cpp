// standart
#include <QApplication>
// scenes
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>

// loaders and other
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <QFile>
#include <QTextStream>

// цвет
struct Color{
    double R;
    double G;
    double B;
    Color (){
        R = 0; G = 0; B = 0;
    }
    Color (double r, double g, double b){
        R = r; G = g; B = b;
    }
    Color operator + (const Color& v) const
        { return Color(R+v.R, G+v.G, B+v.B); }

    void Correct (){
        R = (R > 255.0)? 255.0 : (R < 0.0)? 0.0 : R;
    }
};

// просто точечка
struct Vertex{
    double X;
    double Y;
    double Z;

    Vertex (){
        X = 0; Y = 0; Z = 0;
    }
    Vertex (double x, double y, double z){
        X = x; Y = y; Z = z;
    }
    Vertex operator + (const Vertex& v) const
        { return Vertex(X+v.X, Y+v.Y, Z+v.Z); }
};

struct Polygon{
    Vertex* pps[3];
    double parametric [4];
    Vertex center;
    Vertex normal;
    double mirror_mult;

    Polygon (){
        parametric[0] = 0;
        parametric[1] = 0;
        parametric[2] = 0;
        parametric[3] = 0;
        center = Vertex();
        normal = Vertex();
        mirror_mult = 0;
    }

    void Draw(QGraphicsScene* scene);
    void CalculateParametric();
    void CalculateNormal();
    double GetValue (Vertex* point);
    Vertex InterSect (Vertex* point_from, Vertex* point_to);
    bool RayCastBy (Vertex* point_from, Vertex* point_to, Vertex& intersection);
    bool PointInTriangle (Vertex* point);
};

struct LightSourse{
    Vertex center;
    double max_dist;
    short min_light;
    short max_light;

    void DrawLight (QGraphicsScene *scene);
};

struct Camera {
    Vertex center;  // first point
    Vertex watch;   // second point
};


void DrawLine (Vertex* a, Vertex*b, QColor color, QGraphicsScene* scene);

void DrawPoint (Vertex*a, QColor color, QGraphicsScene* scene);


void LoadModel (QString path, bool swap_Y_Z, double scale,
                   int& point_count, Vertex all_points[],
                   int& polygon_count, Polygon polygons[]);

int main (int argc, char *argv[]){

    return 0;
}




void LoadModel (QString path, bool swap_Y_Z, double scale,
                   int& point_count, Vertex all_points[],
                   int& polygon_count, Polygon polygons[]){

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
                    if (swap_Y_Z == true)std::swap(coords[1],coords[2]);

                    all_points[point_count++] = {coords[0]*scale,coords[1]*scale,coords[2]*scale};

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
