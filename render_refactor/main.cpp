#include "mainwindow.h"
#include <QApplication>
#include "color.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>

double epsilon = .0001;

short screen_width;
short screen_height;
Color** pixels;
bool matrix_is_created = false;

void LoadScene();
Color RenderPixel (short i, short j);
void RenderScene ();

class Vertex{
    // so can be NULL
public:
    double X;
    double Y;
    double Z;
    Vertex (){
        X = 0; Y = 0; Z = 0;
    }
    Vertex (double x, double y, double z){
        X = x; Y = y; Z = z;
    }
    /*Vertex operator +(const Vertex* another) const{
        return Vertex(X + another->X, Y + another->Y, Z + another->Z);
    }
    Vertex operator *(const double K) const{
        return Vertex(X * K, Y * K, Z * K);
    }
    double operator *(const Vertex* another) const{
        return sqrt(pow(another->X - X,2) + pow(another->Y - Y, 2) + pow(another->Z - Z, 2));
    }*/
};
Vertex Summ (const Vertex* vrt) ;
Vertex Mult (const double k) ;
double Dist (const Vertex* vrt) ;

double angle_betw (const Vertex *a, const Vertex *b, const Vertex *c, const Vertex *d);
double angle_betw (const Vertex*a, const Vertex*b, const Vertex*c);

class Triangle{
private:
    // geometric
        Vertex *pps[3];  // массив ссылок на точки
        double parametric[4]; // уравнения плоскости
public:
    // geometric
        Vertex normal_start;
        Vertex normal_end;
    // color
        double mirror;
        Color color_self;
    // methods
        void CalculateParametric();
        void CalculateNormal(bool reverse_normal);
        double GetValue(const Vertex* point);
        Vertex InterSect (const Vertex* from, const Vertex* to);
        bool RayCastBy (const Vertex* point1, const Vertex* point2, Vertex& intersection);
        bool PointInTriangle (const Vertex* point);
        Color GetColor (const Vertex* point);
        void Trace(QGraphicsScene* Scene);
    // constructors
        Triangle ( Vertex *points[] ){
            for (short i=0; i<3; i++)
                pps[i] = points[i];
            // count normals n other
            CalculateNormal(false);
            CalculateParametric();
            mirror = 0.0;
            color_self = Color(255, 255, 255);
        }
        Triangle ( Vertex *points[], double mirr, Color clr){
            for (short i=0; i<3; i++)
                pps[i] = points[i];
            // count normals n other
            CalculateNormal(false);
            CalculateParametric();
            mirror = mirr;
            color_self = clr;
        }
};

class Sphere{
private:
    // geometric
        Vertex center;
public:
    // geometric
        double Rad;
    // colors
        double mirror;
        Color color_self;
    // methods
        bool RayCastBy (const Vertex *point1, const Vertex *point2, Vertex& intersection);
        Color GetColor (const Vertex* point);
    // constructors
        Sphere (Vertex cent, double rad){
            center = cent; Rad = rad;
            mirror = 0; color_self = Color(255,255,255);
        }
        Sphere (Vertex cent, double rad, double mirr, Color clr){
            center = cent; Rad = rad;
            mirror = mirr; color_self = clr;
        }
};


// all polygons
Vertex p1 = Vertex(0, -5, 5), p2 = Vertex(0, 5, 5), p3 = Vertex (0, 20, -15);
Vertex* pp[3] = {&p1, &p2, &p3};
Triangle tt = Triangle(pp);
// all spheres

// all light

int main(int argc, char *argv[])
{
    // make a scene



    // render a scene
    RenderScene();
    // show GUI
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}







// render draw promles
Color** MainWindow :: ScreenMatrix (){
    return pixels;
}
short MainWindow :: scr_width(){
    return screen_width;
}
short MainWindow :: scr_heigh(){
    return screen_height;
}
void MainWindow :: ClearMatrix(){
    for (short i = 0; i < screen_height; i++)
        free(pixels[i]);
    free(pixels);
    return;
}

Vertex Summ( const Vertex* vrt0, const Vertex* vrt){
    return Vertex(vrt0->X + vrt->X, vrt0->Y + vrt->Y, vrt0->Z + vrt->Z);
}
Vertex Middle (const Vertex* vrt0,const Vertex* vrt1,const Vertex* vrt2 ){
    return Vertex((vrt0->X + vrt1->X + vrt2->X) / 3.0,
                  (vrt0->Y + vrt1->Y + vrt2->Y) / 3.0,
                  (vrt0->Z + vrt1->Z + vrt2->Z) / 3.0);
}
Vertex Mult (const Vertex* vrt0, const double k){
    return Vertex(vrt0->X * k, vrt0->Y * k, vrt0->Z * k);
}
double Dist (const Vertex* vrt0,const Vertex* vrt){
    return sqrt(pow(vrt0->X - vrt->X, 2.0) + pow(vrt0->Y - vrt->Y,2.0) + pow(vrt0->Z - vrt->Z,2.0));
}
double Scalar (const Vertex* a, const Vertex*b, const Vertex*c, const Vertex* d){
    return (a->X - b->X)*(c->X - d->X)+(a->Y - b->Y)*(c->Y - d->Y)+(a->Z - b->Z)*(c->Z - d->Z);
}

void Triangle::CalculateParametric(){
    double  x1 = pps[0]->X, x2 = pps[1]->X, x3 = pps[2]->X,
            y1 = pps[0]->Y, y2 = pps[1]->Y, y3 = pps[2]->Y,
            z1 = pps[0]->Z, z2 = pps[1]->Z, z3 = pps[2]->Z;
    parametric[0] = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);//y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
    parametric[1] = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);//z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
    parametric[2] = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);// x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    parametric[3] = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));//-(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));
}

void Triangle::CalculateNormal(bool reverse_normal){
    // высчитывает нормаль полигона.
    // также выставляет знаения поля center_node && normal_node
    double kX = (pps[0]->Y - pps[1]->Y)*(pps[1]->Z - pps[2]->Z) - (pps[0]->Z - pps[1]->Z)*(pps[1]->Y - pps[2]->Y);
    double kY = (pps[0]->X - pps[1]->X)*(pps[1]->Z - pps[2]->Z) - (pps[0]->Z - pps[1]->Z)*(pps[1]->X - pps[2]->X);
    double kZ = (pps[0]->X - pps[1]->X)*(pps[1]->Y - pps[2]->Y) - (pps[1]->X - pps[2]->X)*(pps[0]->Y - pps[1]->Y);

    double kMax = std::max(abs(kX),std::max(abs(kY),abs(kZ)));
    if (kMax == 0) return;
    Vertex add = Vertex(kX / kMax, kY / kMax, kZ / kMax);
    add = Mult (&add, (reverse_normal)? -1.0 : 1.0);
    normal_start = Middle (pps[0], pps[1], pps[2]);
    normal_end = Summ ( &normal_end,  &add);
    return;
}
bool Triangle::PointInTriangle (const Vertex* point){

    double diff_angle =  2.0 * M_PI - (
                         angle_betw(pps[0],point, pps[1]) +
                         angle_betw(pps[1],point, pps[2]) +
                         angle_betw(pps[2],point, pps[0]));
    return ((diff_angle != diff_angle && (Dist(pps[0],pps[1]) + Dist(pps[1],pps[2]) + Dist(pps[2],pps[0])) * 0.71 >  //0.71
            Dist(pps[0],point) + Dist(pps[1],point) + Dist(pps[2],point)) ||
            ( diff_angle > -epsilon && diff_angle < epsilon));
}
double angle_betw (const Vertex *a, const Vertex *b, const Vertex *c, const Vertex *d){
    return acos((double)Scalar(a,b,c,d) / (Dist(a,b)*Dist(c,d))); // to degrees
}
double angle_betw (const Vertex *a, const Vertex *b, const Vertex *c){
    double res = angle_betw(a, b, b,c);
    return M_PI - res;
}

Color RenderPixel(short i, short j){
    //return Color( (i+j) % 255);;
    Vertex v = Vertex (0, i / 9.0 - 20.0, j / 9.0 - 20.0);
    if (tt.PointInTriangle(&v))
        return Color(255,120,0);

    return Color(20.0,20.0, 255.0);
}

void RenderScene (){
    screen_width = 400;
    screen_height = 400;
    // обявление массива пикселей
    if (!matrix_is_created)
        pixels = (Color**) malloc (screen_height * (sizeof(Color*)));
    // выделили память под высоту
    for (short i = 0; i < screen_height; i++){
        if (!matrix_is_created)
            pixels[i] = (Color*)malloc( screen_width * (sizeof(Color)) );
        for (short j = 0; j < screen_width; j++){
            pixels[i][j] = RenderPixel(i,j);
            // присваивание
        }
    }
    matrix_is_created = true;
    return;
}

