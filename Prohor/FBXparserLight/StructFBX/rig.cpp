#include "rig.h"
#include "QVector2D"
#include "QVector4D"

Rig::Rig()
{
    bindMesh = NULL;
    skeleton = NULL;
    skin = NULL;

}

Rig::Rig(Mesh *mesh, Skeleton *skel, Skin *sk)
{
    bindMesh = mesh;
    skeleton = skel;
    skin = sk;
}

// ____________drawing____________
QVector<QVector2D> From3DTo2D (const QVector<QVector3D> Input3D, const QMatrix4x4 view, const QMatrix4x4 perspective){
    QVector<QVector2D> resultPoints = {};
    for (int curPoint = 0; curPoint < Input3D.length(); curPoint++){
        QVector4D temp = QVector4D(Input3D[curPoint].x(), Input3D[curPoint].y(), Input3D[curPoint].z(), 1.0);
        temp = temp * view * perspective;
        resultPoints << QVector2D(temp.x() / temp.w(), temp.y() / temp.w());
    }
    return resultPoints;
}

bool ApplyScreen (int& x, int& y, const QVector2D point, const int screenWidth, const int screenHeight){
    x = screenWidth/2 + screenWidth/2 * (point[0]);
    y = screenHeight * 3 /4 - screenHeight/2 * (point[1]);
    if (x < 0 || y < 0 || x > screenWidth || y > screenHeight)
        return false;
    return true;
}

QPen ChangeQPainter (QColor clr, int width){
    QPen res;
    res.setWidth(width);
    res.setColor(clr);
    return res;
}
float ang = 0;
QString Rig::ApplyDrawToCanvas(QPainter *painter, const QMatrix4x4 view, const QMatrix4x4 perspective, const int width, const int hei)
{
    skeleton->SetRotation(QVector3D(0, -90, 0), 22);
    skeleton->CalculateGlobalCoordForEachJoint();


    // vertexes
    // ...
    painter->setPen(ChangeQPainter(Qt::red, 3));

    QVector<QVector2D> Vertexes2D = From3DTo2D(bindMesh->vertexes, view, perspective);
    for (int curPoint = 0; curPoint < Vertexes2D.length(); curPoint++)
    {
        int x,y;
        if (ApplyScreen(x,y, Vertexes2D[curPoint], width, hei))
            painter->drawPoint(x,y);
    }

    // joints
    // ...
    QVector<QVector3D> Joints3D;
    for (int curJoint = 0; curJoint < skeleton->joints.length(); curJoint++){
        QVector3D parentVect = QVector3D(0,0,0),
                  childVect = skeleton->getJointCoordByIndex(curJoint, parentVect);
        Joints3D << childVect << parentVect;
    }

    Vertexes2D = From3DTo2D(Joints3D, view,perspective);
    painter->setPen(ChangeQPainter(Qt::blue, 2));
    for (int curPoint = 0; curPoint < Vertexes2D.length() / 2; curPoint++)
    {
        int xc,yc,xp,yp;
        ApplyScreen(xc,yc,Vertexes2D[curPoint * 2], width, hei);
        ApplyScreen(xp,yp,Vertexes2D[curPoint * 2 + 1], width, hei);
        painter->drawLine(xc,yc,xp,yp);
        //painter->drawText(xc, yc,300,150,0, QString::number(Joints3D[curPoint*2].x()) +"\n"+ QString::number(Joints3D[curPoint*2].y()) +"\n"+ QString::number(Joints3D[curPoint*2].z()));
    }

    //painter->end();
    return QString();
}
