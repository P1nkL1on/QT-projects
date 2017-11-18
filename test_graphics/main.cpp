//#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <math.h>
#include <QGraphicsLineItem>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <QFile>
#include <QTextStream>


//using QIODevice::Text;
using namespace std;
using std::cout;

double zoom = 10;
double X_mult = 0.5;

struct TPoint
{
    double X;
    double Y;
    double Z;

    TPoint (){
        X = 0;
        Y = 0;
        Z = 0;
    }
    TPoint (double x, double y, double z){
        X = x; Y = y; Z = z;
    }
};

void DrawLine ( TPoint *a,  TPoint *b, QColor color, QGraphicsScene *Scene){
    QGraphicsLineItem * line = new QGraphicsLineItem(a->Y*zoom + a->X * zoom * X_mult , -a->Z*zoom - a->X * zoom * X_mult,
                                                     b->Y*zoom + b->X * zoom * X_mult, -b->Z*zoom - b->X * zoom * X_mult);
//    QGraphicsLineItem* line = new QGraphicsLineItem(a->X * zoom, a->Y * zoom, b->X * zoom, b->Y * zoom);

    QPen pen;
    pen.setColor(color);
    line->setPen(pen);
    Scene->addItem(line);
    //double c = a->X + b->X;
}

void DrawLine ( TPoint *a,  TPoint *b, QGraphicsScene *Scene){
    DrawLine (a,b,QColor(Qt::black), Scene);
}

const double point_cross = 4;
void DrawPoint (TPoint* a, QGraphicsScene *Scene){
    QGraphicsLineItem *
            line = new QGraphicsLineItem(a->Y*zoom + a->X * zoom * X_mult - point_cross, -a->Z*zoom- a->X * zoom * X_mult, a->Y*zoom + a->X * zoom * X_mult + point_cross, -a->Z*zoom- a->X * zoom * X_mult);
    Scene->addItem(line);
            line = new QGraphicsLineItem(a->Y*zoom + a->X * zoom * X_mult, -a->Z*zoom + point_cross- a->X * zoom * X_mult, a->Y*zoom + a->X * zoom * X_mult, -a->Z*zoom - point_cross- a->X * zoom * X_mult);
    Scene->addItem(line);

}

TPoint Summ (TPoint *o, double x_offset, double y_offset, double z_offset){
    return {o->X + x_offset,o->Y + y_offset, o->Z + z_offset};

}

double Dist (TPoint *a, TPoint *b){
    return sqrt(pow(a->X - b->X,2)+pow(a->Y - b->Y,2)+pow(a->Z - b->Z,2));
}

// found AB || CD ugol
double angle_betw (TPoint *a, TPoint *b, TPoint *c, TPoint *d){
    double cos_chis =
        (a->X - b->X)*(c->X - d->X)+(a->Y - b->Y)*(c->Y - d->Y)+(a->Z - b->Z)*(c->Z - d->Z);
    double cos_znam =
         sqrt(pow((a->X - b->X),2)+pow((a->Y - b->Y),2)+pow((a->Z - b->Z),2))
        *sqrt(pow((c->X - d->X),2)+pow((c->Y - d->Y),2)+pow((c->Z - d->Z),2));
    double res =  acos((double)cos_chis / (double)cos_znam); // to degrees

    return res;
}

double angle_betw (TPoint *a, TPoint *b, TPoint *c){
    return M_PI - angle_betw(a,b,b,c);
}

TPoint ScalarMultiply(TPoint *sourse, double scalar)
   {
    return {sourse->X * scalar, sourse->Y * scalar, sourse->Z * scalar};
   }

struct LightSourse{

    TPoint center;
    double max_dist;
    short min_light;
    short max_light;
    //unsigned short color[3];

    void DrawLight (QGraphicsScene *Scene){
        DrawPoint(&center, Scene);
    }
};
struct TSphere;
// include parametric inself
struct TTriangle
{
    TPoint *pps[3];
    double parametric[4];
    TPoint center_node;
    TPoint normal_node;
    double mirror;
    QColor self_color;

    void DrawTriangle (QGraphicsScene *Scene){
        DrawLine(pps[0], pps[1], Scene);
        DrawLine(pps[1], pps[2], Scene);
        DrawLine(pps[2], pps[0], Scene);
        //DrawLine(&center_node, &normal_node, {Qt::red}, Scene);
    }
    void CalculateParametric (){

        double  x1 = pps[0]->X, x2 = pps[1]->X, x3 = pps[2]->X,
                y1 = pps[0]->Y, y2 = pps[1]->Y, y3 = pps[2]->Y,
                z1 = pps[0]->Z, z2 = pps[1]->Z, z3 = pps[2]->Z;
        parametric[0] = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);//y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
        parametric[1] = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);//z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
        parametric[2] = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);// x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
        parametric[3] = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));//-(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1));

    }
    void CalculateNormal (bool reverse_normal){
        // высчитывает нормаль полигона.
        // также выставляет знаения поля center_node && normal_node

        double kX = (pps[0]->Y - pps[1]->Y)*(pps[1]->Z - pps[2]->Z) - (pps[0]->Z - pps[1]->Z)*(pps[1]->Y - pps[2]->Y);
        double kY = (pps[0]->X - pps[1]->X)*(pps[1]->Z - pps[2]->Z) - (pps[0]->Z - pps[1]->Z)*(pps[1]->X - pps[2]->X);
        double kZ = (pps[0]->X - pps[1]->X)*(pps[1]->Y - pps[2]->Y) - (pps[1]->X - pps[2]->X)*(pps[0]->Y - pps[1]->Y);

        double kMax = max(abs(kX),max(abs(kY),abs(kZ))) * 1;
        if (kMax == 0) return;
        kX /= kMax; kY /= kMax; kZ /= kMax;
        center_node = {(pps[0]->X+pps[1]->X+pps[2]->X)/3.0,
                       (pps[0]->Y+pps[1]->Y+pps[2]->Y)/3.0,
                       (pps[0]->Z+pps[1]->Z+pps[2]->Z)/3.0,};

        int k = (reverse_normal == true)? -1 : 1;
        normal_node = {center_node.X + kX * k, center_node.Y - kY * k, center_node.Z + kZ * k};
    }

    double GetValue (TPoint* point){
        return parametric[0] * point->X + parametric[1] * point->Y + parametric[2] * point->Z +
                        parametric[3];
    }
    TPoint InterSect (TPoint *point1, TPoint *point2){
        double distance = sqrt(pow(point1->X - point2->X,2)+
                               pow(point1->Y - point2->Y,2)+
                               pow(point1->Z - point2->Z,2));   // distance betw 2 poitns
        TPoint cosPoint = {(point2->X - point1->X)/distance,
                           (point2->Y - point1->Y)/distance,
                           (point2->Z - point1->Z)/distance};   // spec point
        double
            point1Value = GetValue(point1);
        double
            cosValue = GetValue(&cosPoint) - parametric[3];
        if (cosValue != 0)
        {
            TPoint fin = ScalarMultiply(&cosPoint, -(point1Value / (double)cosValue));
            return {point1->X + fin.X, point1 ->Y + fin.Y, point1->Z + fin.Z};
        }
            //return Consts.vertexSummary(point1, Consts.scalarMultiply(cosPoint, -(point1Value / cosValue)));
        return *point2;
        /*
            Vertex
                point1 = pointLine.PointFrom,
                point2 = pointLine.PointTo;
            double
                distance = Consts.Dist(point1, point2);//point1.Distance(point2);
            Vertex
            cosPoint = new Vertex(
                (point2.X - point1.X) / distance,
                (point2.Y - point1.Y) / distance,
                (point2.Z - point1.Z) / distance);
            double
                point1Value = this.GetValue(point1),
                cosValue = this.GetValue(cosPoint) - this.d;
            //if ((cosValue > Double.Epsilon) && (point1Value > Double.Epsilon))
            if (cosValue != 0)
                return Consts.vertexSummary(point1, Consts.scalarMultiply(cosPoint, -(point1Value / cosValue)));//point1.Add(Consts.scalarMultiply(cosPoint,(point1Value / cosValue)));
            return new Vertex(0, 0, 0);
          */

    }
    bool RayCastBy (TPoint *point1, TPoint *point2, TPoint& intersection){

        // если точка, из которой выпущен луч находится в самом полигоне,
        // то луч этот полигон не пересекает (будем считать)
        // это необходимо, чтобы луч, выпущенный к источнику света
        // из полигона не стукался об этот самый же полигон !!!!

        //убрать, если будет что то плохо работать
        //if (PointInTriangle(point1))return false;
        // высчитывается точка пересечения луча и параметрической плоскости
        TPoint intersect_point = InterSect(point1, point2);
        // если расстояние междуфинальной точкой и сечением меньше, чем м-ду началом и концом, то
        // все выглядит так *--------/----------------->
        if (Dist(&intersect_point, point2) > Dist(point1, point2))
        {
            //тогда пересечения никакого нет
            intersection = *point1;return false;
        }
        // параметром возвращается сама точка пересечения
        intersection = intersect_point;
        // возвращает принадлежность точки пересечения луча и плоскости полигону
        return PointInTriangle(&intersect_point);

    }
    bool PointInTriangle (TPoint* point){
        // intersect point is in plane of polygon
        double total_angle = angle_betw(pps[0],point, pps[1]) +
                             angle_betw(pps[1],point, pps[2]) +
                             angle_betw(pps[2],point, pps[0]);

        //                                        checking NAN value
        return ((abs (total_angle - 2 * M_PI) < .01) ||
                  // внутре по угольной системе
                (total_angle != total_angle &&  // если НаН, то проверять по периметру
                (Dist(pps[0],pps[1]) + Dist(pps[1],pps[2]) + Dist(pps[2],pps[0])) * 0.71 >  //0.71
                Dist(pps[0],point) + Dist(pps[1],point) + Dist(pps[2],point)));

    }

    QColor GetColor (TTriangle* , int , TSphere* , int , LightSourse* , int , TPoint*);
};

// include a sphere primitive
struct TSphere
{
    TPoint* center_node;
    double Rad;
    double mirror;
    QColor self_color;

    TPoint get_normal (TPoint* in){

        double leng = Dist(in, center_node);
        TPoint res = TPoint{in->X + (in->X - center_node->X)  / leng ,
                            in->Y + (in->Y - center_node->Y)  / leng ,
                            in->Z + (in->Z - center_node->Z)  / leng };
        return res;
    }
    bool RayCastBy (TPoint *point1, TPoint *point2, TPoint& intersection){

          double a = pow((point2->X - point1->X),2) + pow((point2->Y - point1->Y),2) + pow((point2->Z - point1->Z),2);
          double c = pow((point1->X - center_node->X),2) + pow((point1->Y - center_node->Y),2) + pow((point1->Z - center_node->Z),2) - Rad * Rad;
          double b = 2*((point2->X - point1->X) * (point1->X - center_node->X) +
                        (point2->Y - point1->Y) * (point1->Y - center_node->Y) +
                        (point2->Z - point1->Z) * (point1->Z - center_node->Z));
          double Disc = b * b  - 4 * a *c;
          if (Disc >= 0){
              if (Disc < .1)
              {
                  // единственная точка пересечения с коэф-ом
                  double d = -b / (2 * a);
                  intersection = {point1->X + (point2->X - point1->X) * d,
                                  point1->Y + (point2->Y - point1->Y) * d,
                                  point1->Z + (point2->Z - point1->Z) * d};
              }
              else
              {
                  // 2 точки пересечения выбираем лижайшую к камере
                  double d1 = (-b + sqrt(Disc)) / (2 * a);
                  double d2 = (-b - sqrt(Disc)) / (2 * a);
                  TPoint int1n =   {point1->X + (point2->X - point1->X) * d1,
                                    point1->Y + (point2->Y - point1->Y) * d1,
                                    point1->Z + (point2->Z - point1->Z) * d1};
                  TPoint int2n =   {point1->X + (point2->X - point1->X) * d2,
                                    point1->Y + (point2->Y - point1->Y) * d2,
                                    point1->Z + (point2->Z - point1->Z) * d2};
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

    QColor GetColor (TTriangle* polygons, int, TSphere* spheres, int,
                     LightSourse* lights, int light_source_count,
                     TPoint* current_intersect);
};

TPoint RayCast (TPoint from, TPoint to, TTriangle* polygons, int polygon_count,
                double& distance, TTriangle& founded){

    TPoint ox = {-10000, -10000, -10000}, oy = {-10001, -10000, -10000}, oz = {-10000, -10001, -10000};
    founded = TTriangle{&ox, &oy, &oz};

    TPoint current_ray_begin = from;
    TPoint current_ray_end = to;
    TPoint current_intersect = {current_ray_end.X,current_ray_end.Y,current_ray_end.Z};
    // check intersectino
    for (int pol = 0; pol < polygon_count; pol++, polygons++){
        TPoint new_inter = from;
        bool intersection_found = polygons->RayCastBy
                (&current_ray_begin, &current_ray_end, new_inter);
        if (intersection_found){    // intersection_found
                //DrawLine(&new_inter, &current_ray_end, QColor (Qt::blue), Scene);
                double dist_new = Dist(&current_ray_begin, &new_inter);
                double dist_was = Dist(&current_ray_begin, &current_intersect);

                    if (dist_new < dist_was && dist_new > .001)
                    {
                        current_intersect = new_inter;
                        distance = Dist(&current_ray_begin, &current_intersect);
                        founded = *polygons;
                    }
            }

        }
    return current_intersect;
}

TPoint RayCastSphere (TPoint from, TPoint to, TSphere* spheres, int sphere_count,
                double& distance, TSphere& founded){

    TPoint ox = {-10000, -10000, -10000}; double rad = .1;
    founded = TSphere{&ox, rad};

    TPoint current_ray_begin = from;
    TPoint current_ray_end = to;
    TPoint current_intersect = {current_ray_end.X,current_ray_end.Y,current_ray_end.Z};
    // check intersectino
    for (int pol = 0; pol < sphere_count; pol++, spheres++){
        TPoint new_inter = from;
        bool intersection_found = spheres->RayCastBy
                (&current_ray_begin, &current_ray_end, new_inter);
        if (intersection_found){    // intersection_found
                //DrawLine(&new_inter, &current_ray_end, QColor (Qt::blue), Scene);
                double dist_new = Dist(&current_ray_begin, &new_inter);
                double dist_was = Dist(&current_ray_begin, &current_intersect);

                    if (dist_new < dist_was && dist_new > .001)
                    {
                        current_intersect = new_inter;
                        distance = Dist(&current_ray_begin, &current_intersect);
                        founded = *spheres;
                    }
            }

        }
    return current_intersect;
}


void DrawCenter (QGraphicsScene *Scene, float dist){
    DrawLine (new TPoint(0,0,0), new TPoint(-dist,0,0),QColor(Qt::red), Scene);
    for (int i = 0; i < (int) (dist * 2); i++)
        DrawLine (new TPoint(-i / 2.0,.05,0), new TPoint(-i / 2.0,-.05,0), QColor(Qt::red), Scene);
    DrawLine (new TPoint(0,dist,0), new TPoint(0,-dist,0), QColor(Qt::blue), Scene);
    DrawLine (new TPoint(0,0,dist), new TPoint(0,0,-dist), QColor(Qt::green),Scene);

}
TPoint Reflect( TPoint* current_ray_begin, TPoint* current_intersect, TPoint* normal_start, TPoint* normal_end, QGraphicsScene* Scene){
    double dd = Dist(normal_start, normal_end);
    /*if (dd != 1.0)
    {
           TPoint normal_end_new = TPoint{  normal_start->X + (normal_end->X - normal_start->X) / dd,
                                            normal_start->Y + (normal_end->Y - normal_start->Y) / dd,
                                            normal_start->Z + (normal_end->Z - normal_start->Z) / dd};
           normal_end = &normal_end_new;
    }*/
    double scalar = Dist(current_ray_begin, current_intersect) *
                                Dist(normal_start, normal_end)
                 * cos (angle_betw( current_intersect, current_ray_begin, normal_start, normal_end)) * (1.0);
    TPoint res = TPoint{
                current_intersect->X + 2.0 * (+(current_intersect->X - current_ray_begin->X) + 2.0 * scalar * (normal_end->X - normal_start->X)),
                current_intersect->Y + 2.0 * (+(current_intersect->Y - current_ray_begin->Y) + 2.0 * scalar * (normal_end->Y - normal_start->Y)),
                current_intersect->Z + 2.0 * (+(current_intersect->Z - current_ray_begin->Z) + 2.0 * scalar * (normal_end->Z - normal_start->Z))
                };
    // normalize
    int side = 1;
   // return res;
    scalar = 1 / 60.0 * sqrt(pow(res.X - current_intersect->X,2) + pow(res.Y - current_intersect->Y,2) + pow(res.Z - current_intersect->Z,2));
    return TPoint{  current_intersect->X  + side*(res.X - current_intersect->X ) / scalar,
                    current_intersect->Y  + side*(res.Y - current_intersect->Y ) / scalar,
                    current_intersect->Z  + side*(res.Z - current_intersect->Z ) / scalar};
}

struct Camera {
    TPoint center;  // first point
    TPoint watch;   // second point
    bool perspective;
};
QColor use_shader (QColor first_color);
QColor ray_colors[4] = {QColor (Qt::lightGray), QColor(Qt::green), QColor(Qt::yellow), QColor (Qt::red)};
void DrawRayCast (QGraphicsScene *Scene, Camera* camera,
                  TTriangle* polygons, int polygon_count,
                  TSphere* spheres, int sphere_count,
                  LightSourse* lights, int light_source_count,
                  int width, int height, double scan_close, int skip_point,
                  double x_offset, double y_offset){

    int pixel_width = width;   // screen width
    int pixel_height = height;  // screen height
    double distance_camera = Dist(&(camera->center), &(camera->watch));
    double mult = scan_close;

    QGraphicsLineItem * line;
    // border lines
    line = new QGraphicsLineItem(x_offset - pixel_width/2, y_offset - pixel_height/2,x_offset + pixel_width/2, y_offset - pixel_height/2); Scene->addItem(line);
    line = new QGraphicsLineItem(x_offset + pixel_width/2, y_offset - pixel_height/2,x_offset + pixel_width/2, y_offset + pixel_height/2); Scene->addItem(line);
    line = new QGraphicsLineItem(x_offset + pixel_width/2, y_offset + pixel_height/2,x_offset - pixel_width/2, y_offset + pixel_height/2); Scene->addItem(line);
    line = new QGraphicsLineItem(x_offset - pixel_width/2, y_offset + pixel_height/2,x_offset - pixel_width/2, y_offset - pixel_height/2); Scene->addItem(line);
    //Scene->clear();

    TPoint sp_center = TPoint(0,0,0);
    TSphere sp = TSphere{&sp_center, 10.0};
    QPen pen;
    const int max_rays = 10;
    //std::string message = "";
    for (int i = -pixel_width/2; i< pixel_width/2; i++)
        for (int j = -pixel_height/2; j<pixel_height/2; j++)
        {
            // расстояние от начала луча до конца луча
            distance_camera = Dist(&(camera->center), &(camera->watch));
            // дистанция от начала луча до пересечения
            double distace_intersect = distance_camera;
            double distace_intersect_sphere = distance_camera;
            // полигон, пересекаемый
            TTriangle polygon_intersected;
            // сфера, пересекаемая
            TSphere sphere_intersected;
            // начало луча (именно этого) и его конец
            TPoint current_ray_begin = Summ(&(camera->center),0,i * mult,j * mult);
            TPoint current_ray_end = Summ(&(camera->watch),0,i * mult,j * mult);
            if (camera->perspective) current_ray_begin = (camera->center);//{camera->watch.X, 0, 0}; - perspective
            // цвета всех найденных точек и коэффиценты влияния цвета (получены из коэффицента отражения полигона)
            QColor found_colors[max_rays+1]; // все найденные цвета соприкосновений
            double found_multys[max_rays+1]; // все найденные коэффиценты
            // финальный цвет (по дефолту белый), если уходит в бесконечность, то снииний
            QColor final_color = QColor (Qt::white);
            // количество отраженных лучей
            int ray_mirrored = 0;
            // количество наденных точек (может отличаться на 1 от кол-ва лучей (в плюс)
            int points_found = 0;
            // узнать, на что мы наткнулись
            bool isSphere = false;
            do {
                isSphere = false;
                distace_intersect = distance_camera;
                distace_intersect_sphere = distance_camera;
                TPoint current_intersect =
                            RayCast(current_ray_begin, current_ray_end, polygons, polygon_count,
                                    distace_intersect, polygon_intersected);
                TPoint current_sphere_intersect =
                            RayCastSphere(current_ray_begin, current_ray_end, spheres, sphere_count,
                                    distace_intersect_sphere, sphere_intersected);

                // если сфера ближе, чем полигон,
                // то теперь пересечение на сфере

                if (distace_intersect <= distace_intersect_sphere){
                    isSphere = false;
                }else{
                    isSphere = true;
                    current_intersect = current_sphere_intersect;
                    distace_intersect = distace_intersect_sphere;
                }

                if (i% 1 == 0 && j == 0){
                    //DrawLine (&current_ray_begin, &current_intersect, ray_colors[(ray_mirrored < 4)? ray_mirrored : 3], Scene);
                    //DrawLine (&current_ray_begin, &current_ray_begin, QColor(Qt::red), Scene);
                }
                // color of intersect
                QColor new_color;
                points_found++;

                    // мы нашли полигон лдя пересечения
                if (distace_intersect < distance_camera)
                {
                    if (!isSphere){
                           // вызываем функцию полигона
                           new_color = polygon_intersected.GetColor(
                                               polygons, polygon_count,
                                               spheres, sphere_count,
                                               lights, light_source_count, &current_intersect);
                           // добавляем найденный цвет и коффицент  в массив
                           found_colors[ray_mirrored] = new_color;
                           found_multys[ray_mirrored] = polygon_intersected.mirror;
                           found_multys[ray_mirrored + 1] = polygon_intersected.mirror;
                           // задел на отражение неба
                    }else{
                            // вызываем функцию сферы
                            new_color = sphere_intersected.GetColor(
                                                polygons, polygon_count,
                                                spheres, sphere_count,
                                                lights, light_source_count, &current_intersect);
                            // добавляем найденный цвет и коффицент  в массив
                            found_colors[ray_mirrored] = new_color;
                            found_multys[ray_mirrored] = sphere_intersected.mirror;
                            found_multys[ray_mirrored + 1] = sphere_intersected.mirror;
                            // задел на отражение неба
                    }
                }else{
                   found_colors[ray_mirrored] = QColor(0.0,0.0,255.0);
                   found_multys[ray_mirrored] = 1.0 - found_multys[ray_mirrored];
                }
                //final_color = new_color;
                // mirror
                // POLYGON
                if (!isSphere && polygon_intersected.center_node.X > - 1000 && polygon_intersected.mirror > .0){
                        TPoint pp = Reflect (&current_ray_begin, &current_intersect,
                                             &(polygon_intersected.center_node), &(polygon_intersected.normal_node), Scene);

                        current_ray_begin = current_intersect;
                        current_ray_end = pp;
                        distance_camera = Dist(&(current_intersect), &(pp));
                        //distace_intersect = distance_camera;

                        ray_mirrored++;
                   }
                // SPHERE
                if (isSphere && sphere_intersected.center_node->X > - 1000 && sphere_intersected.mirror > .0){
                        TPoint normal_node = sphere_intersected.get_normal(&current_intersect);
                        //if (Dist(&normal_node))
                        TPoint pp = Reflect (&current_ray_begin, &current_intersect,
                                             &current_intersect, &(normal_node), Scene);

                        //if (j == 0 && i > 0 && ray_mirrored == 0){
                            //cout << "(" << i << ")"<< dist1 << " <  " << dist2<<endl;
                            //DrawLine (&current_intersect, &pp, QColor(Qt::blue), Scene);
                            //DrawLine (&current_intersect, &pp_alt, QColor(Qt::yellow), Scene);
                        //}


                        current_ray_begin = current_intersect;
                        current_ray_end = pp;
                        distance_camera = Dist(&(current_intersect), &(pp));

                        ray_mirrored++;
                }
            } while
            (ray_mirrored  < max_rays && (
                    (!isSphere && polygon_intersected.center_node.X > - 9000.0 && polygon_intersected.mirror > 0.0) ||
                    (isSphere && (sphere_intersected.center_node->X > -9000.0) && sphere_intersected.mirror > 0.0)));

            // обратным путем пытаемсся восстановить цвет пикселя
            final_color = found_colors[0];

            if (points_found > 1)
                for (int cl = points_found - 1; cl >= 0; cl -- ){
                    QColor q = QColor    ((double)(found_colors[cl].red()) * (1.0 - found_multys[cl]) + (double)(final_color.red()) * (found_multys[cl] ),
                                         (double)(found_colors[cl].green()) * (1.0 - found_multys[cl]) + (double)(final_color.green()) * (found_multys[cl] ),
                                         (double)(found_colors[cl].blue()) * (1.0 - found_multys[cl]) + (double)(final_color.blue()) * (found_multys[cl] ));
                    final_color = q;
                }


            if (j == 0 &&  i % ((pixel_width) / 100) == 0){
                cout << 50 + i / ((pixel_width + 1) / 100)<< '%'<< endl;
            }
            //cout <<i << ", " << j << " : " << ray_mirrored << endl;

            // final
            int pixel_x = x_offset  + i, pixel_y = y_offset  - j;
            line = new QGraphicsLineItem(pixel_x,pixel_y,pixel_x,pixel_y);
            pen.setColor(use_shader(final_color));
            line->setPen(pen);
            Scene->addItem(line);
        }
}


void LoadModel (QString path,
                bool swap_Y_Z,      // поменять Y и Z координаты
                double scale,
                                  int& point_count, TPoint all_points[],
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene * scene = new QGraphicsScene();

    // loading a model
    int point_count = 0;
    TPoint ps[1000];
    TPoint* pp [1000];
    int sphere_count = 0;
    TSphere sf[100];
    int polygon_count = 0;
    TTriangle tt[2000];

    // lightsources
    LightSourse ls[3] = {{{0.0, 0.0, 50},250.0,0,255}, {{ -55,25, 25},150.0, 0, 255}, {{ 30,25, 25},150.0, 0, 255}};
    LightSourse* rls[3] = {&(ls[0]), &(ls[1]), &(ls[2])};
    //Camera camera = {{-30.0,0,0},{45.0,0,0}, false};
    Camera camera = {{-20, 0, 0}, {60, 0, 0}, true};
    Camera* cm = &camera;
    scene->clear();

    LoadModel("D:/QT-projects/QT-projects/!models/cube.txt", false, 1,
               point_count, ps, polygon_count, tt);
    std::cout<<"Point count: "<<point_count << "  Polygon count:"<<polygon_count<< endl;

    for (int j =0; j<polygon_count; j++)
        {tt[j].CalculateParametric(); tt[j].CalculateNormal( false );        // <-------false
         tt[j].mirror = .0; tt[j].self_color = QColor(Qt::white);}

    // creating a plane with different colors
    //spheres
    // sphere creating
    TPoint sp1_c = {0,-8,0}, sp2_c = {0, 8, 0}, sp3_c = {0,0, -10}, sp4_c = {0, -5, 5}, sp5_c = {0, -3, 0};
    sf[0] = TSphere{&sp1_c, 5, 0.5, QColor(Qt::white)};
    sf[1] = TSphere{&sp2_c, 5, 0.5, QColor(Qt::green)};
    sf[2] = TSphere{&sp3_c, 8, 0.5, QColor(Qt::yellow)};
    sf[3] = TSphere{&sp4_c, 4.5, 0.8};
    sf[4] = TSphere{&sp5_c, 2, 0.5, QColor(Qt::black)};
    sphere_count = 5;

    zoom = 20;
    X_mult = 0.0;
    DrawCenter(scene,6);
    for (int j =0; j<polygon_count; j++)
        tt[j].DrawTriangle(scene);
    for (int j=0; j<2; j++)
        ls[j].DrawLight(scene);

    //polygon_count
    //if (false)
    DrawRayCast (scene, cm,
                 tt, polygon_count,
                 sf, sphere_count,
                 ls, sizeof(ls)/sizeof(ls[0]),
            1000, 1000, .06, 1, 0, -300);


    //sizeof(ls)/sizeof(ls[0])
    QGraphicsView * view = new QGraphicsView(scene);
    // создаетсявьювер этой сцены и с него трясется вид
    view->showFullScreen();
    return a.exec();
}



QColor TSphere::GetColor (
                          TTriangle* polygons, int polygon_count,
                          TSphere* spheres, int sphere_count,
                          LightSourse* lights, int light_source_count,
                          TPoint* current_intersect){
    // цвета пикселя (изначально эти цвета черные, но потом накапливаются
    // от источников света ++)
    double R=0, G = 0, B = 0;

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
}

QColor TTriangle::GetColor (
                 TTriangle* polygons, int polygon_count,
                 TSphere* spheres, int sphere_count,
                 LightSourse* lights, int light_source_count,
                 TPoint* current_intersect){
    // цвета пикселя (изначально эти цвета черные, но потом накапливаются
    // от источников света ++)
    double R=0, G = 0, B = 0;

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
                double ang_k =  1 - angle_betw(&(lights[l].center), &(center_node), &(normal_node)) / M_PI;
                if (!(ang_k != ang_k))add_light *= max(0.0,( ang_k * 2.0 - 1.0));

                R += add_light; G += add_light; B += add_light;
            }
    }
    // устанавливаем пиксель на соответствующий цвет и красим. вуаля
    return (QColor{min(254,max(0,(int)(R * this->self_color.red() / 255.0))),
                   min(254,max(0,(int)(G * this->self_color.green() / 255.0))),
                   min(254,max(0,(int)(B * this->self_color.blue() / 255.0)))});
}
QColor use_shader (QColor first_color){

    double K[3]; K[0] = first_color.red(); K[1] = first_color.green(); K[2] = first_color.blue();

    // monochrom
    //    return QColor((R+G+B) / 3.0, (R+G+B) / 3.0, (R + G + B) / 3.0);

    // make it harder
   for (int i = 0; i < 3; i++)
       K[i] = K[i] * 1.8 - 255 * .4;

   return QColor (max(0.0,(min(255.0, K[0]))),max(0.0,(min(255.0, K[1]))),max(0.0,(min(255.0, K[2]))));
}
