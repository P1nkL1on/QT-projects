#include "rig.h"
#include "QVector2D"
#include "QVector4D"

Rig::Rig()
{
    bindMesh = NULL;
    skeleton = NULL;
    skin = NULL;

    bendedMesh = NULL;
}

Rig::Rig(Mesh *mesh, Skeleton *skel, Skin *sk)
{
    bindMesh = mesh;
    skeleton = skel;
    skin = sk;

    bendedMesh = NULL;
}

void Rig::BendSkinToSkeleton()
{
    Q_ASSERT(bindMesh->vertexes.length() == skin->vertAttends.length());
    Mesh* newMesh = new Mesh();
    int vertexesTransformed = 0;
    for (int currentVertexInd = 0; currentVertexInd < skin->vertAttends.length(); currentVertexInd ++){
        QVector<QVector3D> bendedVariants;
        QVector<float> weightes;

        // we have 0.3 && QVec3D && from joint a place and rotation
        for (int jointInd = 0; jointInd < skin->vertAttends[currentVertexInd].jointIndexs.length(); jointInd++){
            int jointBendInd = skin->vertAttends[currentVertexInd].jointIndexs[jointInd];
            QVector3D originalOffset = skin->vertAttends[currentVertexInd].localJointCoords[jointInd];
            QVector3D jointBendTranslation, jointBendRotation;
            skeleton->getJointTranslationAndRotation(jointBendInd, jointBendTranslation, jointBendRotation);

            //qDebug() << currentVertexInd<< jointBendInd<< originalOffset << jointBendTranslation << jointBendRotation;

            bendedVariants << CommonFuncs::AddDirect(jointBendTranslation, -originalOffset, jointBendRotation);
            //bendedVariants << jointBendTranslation - originalOffset;
            weightes << skin->vertAttends[currentVertexInd].weights[jointInd];
        }

        QVector3D result = QVector3D();

        for (int curPoint = 0; curPoint < bendedVariants.length(); curPoint++)
            result = result +  weightes[curPoint] * bendedVariants[curPoint];

        if (bendedVariants.length() > 0)
            vertexesTransformed ++;
        newMesh->vertexes << result;
    }
    //qDebug() << QString::number(vertexesTransformed) + " / " + QString::number(skin->vertAttends.length()) + " / " + QString::number(bindMesh->vertexes.length()) +" vertexes transformed";
    //
    bendedMesh = newMesh;
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
    skeleton->SetRotation(QVector3D(0,0,(++ang)), 0);
    skeleton->CalculateGlobalCoordForEachJoint();
    BendSkinToSkeleton();
    // vertexes
    // ...

    QVector<QVector2D> Vertexes2D = From3DTo2D(bindMesh->vertexes, view, perspective);
    QVector<QVector2D> Vertexes2DBend;
    if (bendedMesh != NULL)Vertexes2DBend = From3DTo2D(bendedMesh->vertexes, view, perspective);

    for (int curPoint = 0; curPoint < Vertexes2D.length(); curPoint++)
    {
        int x,y;
        painter->setPen(ChangeQPainter(QColor(0,255, 0,120), 4));
        if (ApplyScreen(x,y, Vertexes2D[curPoint], width, hei))
            painter->drawPoint(x,y);
        // draw bend
        // ...
        // bended mods
        if (bendedMesh != NULL)
        {
            int xb,yb;
            painter->setPen(ChangeQPainter(QColor(255,150,0,255), 2));
            if (ApplyScreen(xb,yb, Vertexes2DBend[curPoint], width, hei))
                painter->drawPoint(xb,yb);
            //painter->setPen(ChangeQPainter(QColor(255,0,0,10), 1));
            //painter->drawLine(xb,yb,x,y);
        }
        // ...
        // draw a attend
        QVector<QVector3D> attened3D = {};
        QVector<QVector2D> attened2D = {};

        for (int att = 0; att < skin->vertAttends[curPoint].localJointCoords.length(); att++)
            attened3D << bindMesh->vertexes[curPoint] + skin->vertAttends[curPoint].localJointCoords[att];
        attened2D = From3DTo2D(attened3D, view, perspective);
        for (int att = 0; att < attened2D.length(); att++){
            int xa, ya;
            ApplyScreen(xa,ya, attened2D[att], width, hei);
            painter->setPen(ChangeQPainter(QColor(255,0,0,5), 2));
            painter->drawLine(x,y,xa,ya);
        }
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
        painter->drawText(xc, yc,300,150,0, QString::number(curPoint));// +" <<  "+QString::number(skeleton->joints[curPoint]->currentRotation.x()) +", "+ QString::number(skeleton->joints[curPoint]->currentRotation.y()) +", "+ QString::number(skeleton->joints[curPoint]->currentRotation.z()));

        //painter->drawText(xc, yc,300,150,0, QString::number(Joints3D[curPoint*2].x()) +"\n"+ QString::number(Joints3D[curPoint*2].y()) +"\n"+ QString::number(Joints3D[curPoint*2].z()));
    }



    //painter->end();
    return QString();
}
