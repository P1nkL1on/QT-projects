#include "mainwindow.h"
#include <QApplication>
#include "color.h"
#include <iostream>
#include <windows.h>

double epsilon = .0001;

short screen_width;
short screen_height;
Color** pixels;
bool matrix_is_created = false;

void LoadScene();



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
Vertex Summ (const Vertex*,const Vertex) ;
Vertex Summ (const Vertex*,const Vertex*) ;
Vertex Summ (const Vertex,const Vertex) ;
Vertex Summ (const Vertex*, const double X, const double Y, const double Z);
Vertex Diff (const Vertex*, const Vertex*);
Vertex Mult (const Vertex*, const double) ;
Vertex Mult (const Vertex, const double) ;
double Dist (const Vertex*, const Vertex*) ;
Vertex Reflect (const Vertex* current_ray_start, const Vertex* current_ray_end,
                const Vertex* normal_start, const Vertex* normal_end);

double angle_betw (const Vertex *a, const Vertex *b, const Vertex *c, const Vertex *d);
double angle_betw (const Vertex*a, const Vertex*b, const Vertex*c);

struct Triangle{
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

struct Sphere{
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
        Vertex GetNormal (const Vertex* point);
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

struct Light{
public:
    Vertex center;
    double max_dist;
    short max_light;
    short min_light;

    Light (Vertex c, double dist, short min, short max){
        center = c; max_dist = dist; min_light = min; max_light = max;
    }
    Light (Vertex c, double dist){
        center = c; max_dist = dist; min_light = 0; max_light = 255;
    }
    Light (Vertex c){
        center = c; max_dist = 20; min_light = 0; max_light = 255;
    }
};

struct Camera{
public:
    Vertex center;
    Vertex watch;
    double perspective; // 0 - perspective, 1 - no, 2 - backperspective
    double frequency;   // .1 - mean that each pixel will count as .1 x .1 square in camera

    Camera (Vertex cent, Vertex wat, double per, double freq){
        center = cent;
        watch = wat; if (Dist(&wat, &cent) <= 0) watch = Summ(&center, 20, 0, 0);
        perspective = std::max(0.0, std::min(2.0, per));
        frequency = freq;
    }
};

Vertex RayCast (const Vertex* from, const Vertex* to, double& distance, int& itnersected_object_number, char& intersected_object_type );
Color RenderRay (const Vertex* from, const Vertex* to);
void RenderScene (Camera*);


// all polygons
Vertex p1 = Vertex(0, 5, 5), p2 = Vertex(-10, 1, 5), p3 = Vertex (-5, 3, -15),
       p4 = Vertex(15, -5, -12), p5 = Vertex(15, 5,- 9), p6 = Vertex (-5, 0, 15);

Vertex* pp[3] = {&p3, &p2, &p1};
Triangle tt1 = Triangle(pp,  .0, Color(255, 0 ,0));
Vertex* pp2[3] = {&p6, &p5, &p4};
Triangle tt2 = Triangle(pp2, .0, Color(255,255,0));

Triangle* TT[] = {&tt1, &tt2};
// all spheres
Sphere sp1 = Sphere(Vertex(0, 0, 0), 2,     .0, Color(255,255,0));
Sphere sp2 = Sphere(Vertex(0, 0, 4), 2,     .0, Color(255,0,0));
Sphere sp3 = Sphere(Vertex(3, -3, -3), 6,   .6, Color(0,255,0));
Sphere sp4 = Sphere(Vertex(-6, -8, 0), 2,   .0, Color(255));
Sphere sp5 = Sphere(Vertex(-6, -12, 0), 2,  .0, Color(0, 255, 255));
Sphere sp6 = Sphere(Vertex(3, -3, 14), 6,   .6, Color(255));
Sphere sp7 = Sphere(Vertex(3, -3, -16), 6,   .6, Color(255,0,0));

Sphere* SP[] = {&sp1, &sp2, &sp3, &sp4, &sp5, &sp6, &sp7};
// all light
Light   lg1 = Light(Vertex(-10, 0, -10), 20, 0, 255),//Light(Vertex(-10, -5, 0), 20, 0, 400);
        lg2 = Light(Vertex(-10, -10, 0), 20),
        lg3 = Light(Vertex(0, -15, 0), 80),
        lg4 =  Light(Vertex(-10, 0, 10), 20, 0, 255);

Light* LS[] = {&lg1, &lg4 ,&lg2/*&lg3*/};

int main(int argc, char *argv[])
{
    // make a scene


    // set camera
    Camera* cam = new Camera(Vertex(-30, 0, 0), Vertex(30, 0, 0), 1.0, .1);
    // render a scene
    RenderScene(cam);
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
Vertex Summ (const Vertex* vrt, const double X, const double Y, const double Z){
    return Vertex(vrt->X +X, vrt->Y + Y, vrt->Z + Z);
}
Vertex Summ (const Vertex* vrt, const Vertex vrt2){
    return Vertex(vrt->X + vrt2.X, vrt->Y + vrt2.Y, vrt->Z + vrt2.Z);
}
Vertex Summ (const Vertex a, const Vertex b){
    return Vertex(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
}

Vertex Diff (const Vertex* a, const Vertex* b){
    return Vertex(a->X - b->X, a->Y - b->Y, a->Z - b->Z);
}

Vertex Middle (const Vertex* vrt0,const Vertex* vrt1,const Vertex* vrt2 ){
    return Vertex((vrt0->X + vrt1->X + vrt2->X) / 3.0,
                  (vrt0->Y + vrt1->Y + vrt2->Y) / 3.0,
                  (vrt0->Z + vrt1->Z + vrt2->Z) / 3.0);
}
Vertex Mult (const Vertex* vrt0, const double k){
    return Vertex(vrt0->X * k, vrt0->Y * k, vrt0->Z * k);
}
Vertex Mult (const Vertex vrt0, const double k){
    return Vertex(vrt0.X * k, vrt0.Y * k, vrt0.Z * k);
}

double Dist (const Vertex* vrt0,const Vertex* vrt){
    return sqrt(pow(vrt0->X - vrt->X, 2.0) + pow(vrt0->Y - vrt->Y,2.0) + pow(vrt0->Z - vrt->Z,2.0));
}
double Scalar (const Vertex* a, const Vertex*b, const Vertex*c, const Vertex* d){
    return (a->X - b->X)*(c->X - d->X)+(a->Y - b->Y)*(c->Y - d->Y)+(a->Z - b->Z)*(c->Z - d->Z);
}
Vertex Reflect (const Vertex* current_ray_begin, const Vertex* current_intersect,
                const Vertex* normal_start, const Vertex* normal_end){
    double dd = Dist(normal_start, normal_end); // normal length
    double scalar = Dist(current_ray_begin, current_intersect) *
                                Dist(normal_start, normal_end)
                 * cos (angle_betw( current_intersect, current_ray_begin, normal_start, normal_end)) * (1.0);

    Vertex res = Summ (current_intersect, Mult( Summ (Diff(current_intersect, current_ray_begin)
                       , Mult(Diff(normal_end, normal_start), 2.0 * scalar)), 2.0));
    scalar = 1 / 60.0 * Dist( &res, current_intersect );
    return Summ (current_intersect, Mult(Diff(&res, current_intersect), 1.0 / scalar));
    /* counting a scalar
    =TPoint res = TPoint{
                current_intersect->X + 2.0 * (+(current_intersect->X - current_ray_begin->X) + 2.0 * scalar * (normal_end->X - normal_start->X)),
                current_intersect->Y + 2.0 * (+(current_intersect->Y - current_ray_begin->Y) + 2.0 * scalar * (normal_end->Y - normal_start->Y)),
                current_intersect->Z + 2.0 * (+(current_intersect->Z - current_ray_begin->Z) + 2.0 * scalar * (normal_end->Z - normal_start->Z))
                };
    // return res;
     //scalar = 1 / 60.0 * sqrt(pow(res.X - current_intersect->X,2) + pow(res.Y - current_intersect->Y,2) + pow(res.Z - current_intersect->Z,2));

    //    return TPoint{  current_intersect->X  + side*(res.X - current_intersect->X ) / scalar,
    //                    current_intersect->Y  + side*(res.Y - current_intersect->Y ) / scalar,
    //                    current_intersect->Z  + side*(res.Z - current_intersect->Z ) / scalar};*/
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
    normal_end = Summ ( &normal_start,  &add);
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
double Triangle::GetValue (const Vertex* point){
    return parametric[0] * point->X + parametric[1] * point->Y + parametric[2] * point->Z +
                    parametric[3];
}
// если пересечение есть, то возвращает его,
// а если нет, то дает начало луча
Vertex Triangle::InterSect(const Vertex* from, const Vertex* to)
{
    double distance = Dist(from, to);
    Vertex cosPoint = {(to->X - from->X)/distance,
                       (to->Y - from->Y)/distance,
                       (to->Z - from->Z)/distance};   // spec point
    double
        fromValue = GetValue(from);
    double
        cosValue = GetValue(&cosPoint) - parametric[3];
    if (cosValue != 0)
    {
        Vertex fin = Mult(&cosPoint, -(fromValue / (double)cosValue));
        // возвращаем точку пересечения
        return Summ(from, fin);
    }
    // возвращаем точку конца луча
    return *from;
}
// если столкновение есть, то возвращает точку и правду\ложь принадлежность
// если нет или в другой стороне то дает фалс, а пересечение в начале луча
bool Triangle::RayCastBy(const Vertex* from, const Vertex* to, Vertex& intersection){
    Vertex inter = InterSect(from, to);
    if (Dist(&inter, to) > Dist(from, to)){
        intersection = *from; return false;
    }
    intersection = inter;
    return PointInTriangle(&inter);
}

bool Sphere::RayCastBy (const Vertex *point1, const Vertex *point2, Vertex& intersection){

      double a = pow((point2->X - point1->X),2) + pow((point2->Y - point1->Y),2) + pow((point2->Z - point1->Z),2);
      double c = pow((point1->X - center.X),2) + pow((point1->Y - center.Y),2) + pow((point1->Z - center.Z),2) - Rad * Rad;
      double b = 2*((point2->X - point1->X) * (point1->X - center.X) +
                    (point2->Y - point1->Y) * (point1->Y - center.Y) +
                    (point2->Z - point1->Z) * (point1->Z - center.Z));
      double Disc = b * b  - 4 * a *c;
      if (Disc >= 0){
          if (Disc < epsilon)
          {
              // единственная точка пересечения с коэф-ом
              double d = -b / (2 * a);
              intersection = {point1->X + (point2->X - point1->X) * d,
                              point1->Y + (point2->Y - point1->Y) * d,
                              point1->Z + (point2->Z - point1->Z) * d};
              //intersection = Summ (point1, Mult (Summ(point2, Mult(point1, -1.0)), d));
          }
          else
          {
              // 2 точки пересечения выбираем лижайшую к камере
              double d1 = (-b + sqrt(Disc)) / (2 * a);
              double d2 = (-b - sqrt(Disc)) / (2 * a);
              Vertex int1n =   {point1->X + (point2->X - point1->X) * d1,
                                point1->Y + (point2->Y - point1->Y) * d1,
                                point1->Z + (point2->Z - point1->Z) * d1};
              Vertex int2n =   {point1->X + (point2->X - point1->X) * d2,
                                point1->Y + (point2->Y - point1->Y) * d2,
                                point1->Z + (point2->Z - point1->Z) * d2};
              //Vertex int1n = intersection = Summ (point1, Mult (Summ(point2, Mult(point1, -1.0)), d1));
              //Vertex int2n = intersection = Summ (point1, Mult (Summ(point2, Mult(point1, -1.0)), d2));

              // ближайшая
              intersection = (Dist(point1, &int1n) < Dist(point1, &int2n))? int1n : int2n;
              //intersection = {0,0,0};
          }
          // if found in opoosite (wrong) side
          if (Dist(point2, &intersection) > Dist(point1, point2))
            {intersection = *point1; return false;}
          return true;
      }
      intersection = *point1;
      return false;
}

double angle_betw (const Vertex *a, const Vertex *b, const Vertex *c, const Vertex *d){
    return acos((double)Scalar(a,b,c,d) / (Dist(a,b)*Dist(c,d))); // to degrees
}
double angle_betw (const Vertex *a, const Vertex *b, const Vertex *c){
    double res = angle_betw(a, b, b,c);
    return M_PI - res;
}

Vertex Sphere::GetNormal(const Vertex *in){
    double leng = Dist(in, &center);
    Vertex res = Vertex(in->X + (in->X - center.X)  / leng ,
                        in->Y + (in->Y - center.Y)  / leng ,
                        in->Z + (in->Z - center.Z)  / leng );

    return res;
}

Color CSumm (Color a, Color b){
    return Color(a.R() + b.R(), a.G() + b.G(), a.B() + b.B() );
}
Color CMult (Color c, double d){
    return Color( c.R() * d, c.G() * d, c.B() * d );
}
Color Over (Color main, Color over){
    return Color (main.R() * over.R() / 255.0,  main.G() * over.G() / 255.0, main.B() * over.B() / 255.0 );
}

Color Triangle::GetColor(const Vertex* current_intersect){
    /*

    for (int l=0; l<light_source_count; l++){
        double distance_to_light = Dist(&(lights[l].center),current_intersect);
         // полигон, который блокирует перемещение света
            double distance_to_block = distance_to_light;
            TTriangle blocker;
            TPoint light_blocking =
                RayCast(lights[l].center, *current_intersect,
                        polygons, polygon_count, distance_to_block, blocker);
         // полигон, который блокирует перемещение света
            double distance_to_block_sphere = distance_to_light;
            TSphere blocker_sphere;
            TPoint light_blocking_sphere =
                RayCastSphere(lights[l].center, *current_intersect,
                        spheres, sphere_count, distance_to_block_sphere, blocker_sphere);
        // приблизительность значения - из-за теряемой в рассчетах точности
        if (abs(distance_to_block - distance_to_light) < .01 &&
            abs(distance_to_block_sphere - distance_to_light) < .01)
        {
                // если луч упирается в камеру и ему ничего не мешает
                // то вычислим прибавку к освещенности участка пикселя
                double add_light = max(0.0,(lights[l].max_dist - distance_to_light)) / lights[l].max_dist *
                        (lights[l].max_light - lights[l].min_light) + lights[l].min_light;
                // влияние угла, под которым повернут к свету полигон на светотень.
                TPoint normal_node = (this->get_normal(current_intersect));
                double ang_k =  1 - angle_betw(&(lights[l].center), (center_node),
                                   &(normal_node)) / M_PI;
                if (!(ang_k != ang_k))add_light *= max(0.0, ang_k * 2.0 - 1.0);

                R += add_light; G += add_light; B += add_light;
        }
    }


    // устанавливаем пиксель на соответствующий цвет и красим. вуаля
    return (QColor{min(254,max(0,(int)(R * this->self_color.red() / 255.0))),
                   min(254,max(0,(int)(G * this->self_color.green() / 255.0))),
                   min(254,max(0,(int)(B * this->self_color.blue() / 255.0)))});

        Vertex new_intersect;
        bool intersection_found = TT[pol]->RayCastBy(from, to, new_intersect);
        double new_dist = Dist(from, &new_intersect);
        // test for best intersection
        if (intersection_found
                && new_dist > epsilon && new_dist < distance){

*/
    Color final = Color(.0);
    for (int l = 0; l< sizeof(LS)/sizeof(LS[0]); l++){
        double dist_to_light = Dist(&(LS[l]->center), current_intersect),
               dist_to_nearest_blocker = dist_to_light;
        int blocker_number = -1; char fin = ' ';

        Vertex blocker = RayCast(current_intersect ,&(LS[l]->center),
                        dist_to_nearest_blocker, blocker_number, fin);
        if (fin == ' ' && dist_to_nearest_blocker > 0){
            double ang_k =  1 - angle_betw(&(LS[l]->center), &(this->normal_start),
                               &(this->normal_end)) / M_PI;
            double lk =
                    std::max(0.0,(LS[l]->max_dist - dist_to_light)) / LS[l]->max_dist *
                    (LS[l]->max_light - LS[l]->min_light) + LS[l]->min_light;
            lk  *= std::max(0.0, ang_k * 2.0 - 1.0);

            final = CSumm (final, CMult (Color(255.0),lk / 255.0));
        }
    }
    return Over(this->color_self, final);
}
Color Sphere::GetColor(const Vertex *current_intersect){
    Color final = Color(.0);
    for (int l = 0; l< sizeof(LS)/sizeof(LS[0]); l++){
        double dist_to_light = Dist(&(LS[l]->center), current_intersect),
               dist_to_nearest_blocker = dist_to_light;
        int blocker_number = -1; char fin = ' ';

        Vertex blocker = RayCast(current_intersect ,&(LS[l]->center),
                        dist_to_nearest_blocker, blocker_number, fin);
        if (fin == ' ' && dist_to_nearest_blocker > 0){
            Vertex normal_node = (this->GetNormal(current_intersect));
            double ang_k =  1 - angle_betw(&(LS[l]->center), &(this->center),
                               &(normal_node)) / M_PI;
            double lk =
                    std::max(0.0,(LS[l]->max_dist - dist_to_light)) / LS[l]->max_dist *
                    (LS[l]->max_light - LS[l]->min_light) + LS[l]->min_light;
            lk  *= std::max(0.0, ang_k * 2.0 - 1.0);

            final = CSumm (final, CMult (Color(255.0),lk / 255.0));
        }
    }
    return Over(this->color_self, final);
}

/* char conditions
 * P or T = polygon\triangle
 * S or B = sphere\ball
   _ or " " or A or N  === nothing, null, AAAAAAAAAAAAAA!*/

Vertex RayCast(const Vertex* from, const Vertex* to, double& distance,
              int& itnersected_object_number, char& intersected_object_type ){
    // ща будем рэйкаcатить
    // на входе дист = расстояние от начала луча до конца, полигон нуловый, точка результат - в концу
    Vertex res = Vertex(to->X, to->Y, to->Z); // как будто луч дошел до конца
    // FOR POLYGONS
    for (int pol = 0; pol < sizeof(TT) / sizeof(TT[0]); pol++)
    {
        Vertex new_intersect;
        bool intersection_found = TT[pol]->RayCastBy(from, to, new_intersect);
        double new_dist = Dist(from, &new_intersect);
        // test for best intersection
        if (intersection_found
                && new_dist > epsilon && new_dist < distance){
            // если пересечение найдено и расстояние до него меньше, чем дистанция
            // и оно не нудевое (на всякий случай)
            res = new_intersect;
            distance = new_dist;
            itnersected_object_number = pol;
            intersected_object_type = 'T';      // triangle
            // то присваиываем полигон и переприсваиваем длину
        }
    }
    // FOR SPHERES
    for (int sph = 0; sph < sizeof(SP) / sizeof(SP[0]); sph++){
        Vertex new_intersect;
        bool intersection_found = SP[sph]->RayCastBy(from, to, new_intersect);
        double new_dist = Dist(from, &new_intersect);
        // test for best intersection
        if (intersection_found
                && new_dist > epsilon && new_dist < distance){
            // если пересечение найдено и расстояние до него меньше, чем дистанция
            // и оно не нудевое (на всякий случай)
            res = new_intersect;
            distance = new_dist;
            itnersected_object_number = sph;
            intersected_object_type = 'S';      // triangle
            // то присваиываем полигон и переприсваиваем длину
        }
    }

    return res;
}

Color RenderRay(Vertex* from, Vertex* to){
    // дистанция по дефолту - jn ahjv lj ne
    // дефолтное попадание на конце камеры
    // пересеченный по умолчанию полигон - нулловый

    double intersection_dist = Dist(from , to);
    Vertex current_intersection = *to;
    int intersect_obj_number = -1;
    char what_we_are_intersecting = ' ';

    current_intersection = RayCast(from, to, intersection_dist, intersect_obj_number, what_we_are_intersecting);
    if (Dist(&current_intersection, to) > 0){
        if (what_we_are_intersecting == 'T'){
            if (TT[intersect_obj_number]->mirror > 0.0)
            {
                Vertex reflect = Reflect(from, &current_intersection,
                                 &(TT[intersect_obj_number]->normal_start), &(TT[intersect_obj_number]->normal_end));
                return CSumm(
                       CMult(TT[intersect_obj_number]->GetColor( &current_intersection ), 1.0 - TT[intersect_obj_number]->mirror),
                       CMult(RenderRay(&current_intersection, &reflect), TT[intersect_obj_number]->mirror));
            }
            else
                TT[intersect_obj_number]->GetColor( &current_intersection );
        }

        if (what_we_are_intersecting == 'S'){
            if (SP[intersect_obj_number]->mirror > 0.0){
                Vertex normal_end = SP[intersect_obj_number]->GetNormal(&current_intersection);
                Vertex reflect = Reflect(from, &current_intersection,
                                         &current_intersection, &normal_end);
                return CSumm(
                    CMult(SP[intersect_obj_number]->GetColor( &current_intersection ), 1.0 - SP[intersect_obj_number]->mirror),
                    CMult(RenderRay(&current_intersection, &reflect), SP[intersect_obj_number]->mirror));
            }
            else
            return  SP[intersect_obj_number]->GetColor( &current_intersection ) ;
        }
        // if intersect something strange
        return Color(0,0,0);
    }

    return Color(20.0,20.0, 255.0);
}

void RenderScene (Camera* cam){
    short trace_perc = 10; SYSTEMTIME st; GetSystemTime(&st); int ms = st.wSecond * 1000 + st.wMilliseconds;

    screen_width = 800;
    screen_height = 800;
    cam->frequency /= (std::min(screen_width, screen_height) / 400);

    int percent_done = 0;
    // обявление массива пикселей
    if (!matrix_is_created)
        pixels = (Color**) malloc (screen_height * (sizeof(Color*)));
    // выделили память под высоту
    // time to see


    for (short i = 0; i < screen_height; i++){
        if (!matrix_is_created)
            pixels[i] = (Color*)malloc( screen_width * (sizeof(Color)) );
        for (short j = 0; j < screen_width; j++){
            Vertex current_ray_start = Summ(&cam->center, 0.0,
                         (-screen_width / 2 + j) * cam->frequency,
                         (-screen_height / 2 + i) * cam->frequency);
            Vertex current_ray_end = Summ(&cam->watch, 0.0,
                         (-screen_width / 2.0 + j) * cam->frequency * cam->perspective,
                         (-screen_height / 2.0 + i) * cam->frequency * cam->perspective) ;

            // присваивание
            pixels[i][j] = RenderRay(&current_ray_start, &current_ray_end);
            // tracing
            if ( j == screen_width-1 && i % (screen_height / (100) * trace_perc - 1) == 0 && i >0)
            {
                percent_done += trace_perc; GetSystemTime(&st);
                std::cout << percent_done << "%  time elapsed: " << st.wSecond * 1000 + st.wMilliseconds - ms<< " ms."<< std::endl;
                ms = st.wSecond * 1000 + st.wMilliseconds;
            }

        }
    }
    matrix_is_created = true;
    return;
}

