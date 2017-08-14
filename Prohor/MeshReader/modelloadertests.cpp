#include "modelloadertests.h"
#include "modelloader.h"
#include <QDebug>

using namespace ModelLoader;

ModelLoaderTests::ModelLoaderTests(QObject *parent) : QObject(parent)
{

}

void ModelLoaderTests::testNormalModel01(){
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "f 1 2 3";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(errorStr.isEmpty());
}

void ModelLoaderTests::testEmptyString01()
{
    QString s = "";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}

void ModelLoaderTests::testInvalidVertex01()
{
    QString s = "v 1 2 abc";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}

void ModelLoaderTests::testInvalidPolygon01()
{
    // указано не число в индексах
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "f 1 2 babaka";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidPolygon02()
{
    // указано не целое или отрицательное число в индексах
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "f -5 -2 2";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidPolygon03()
{
    // указаны индексы неопределенных точек
    QString s = "v 1 2 3\n"
                "f 1 2 3"; // точка то всего одна
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidPolygon04()
{
    // указаны индексы неопределенных точек
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "f 3 2 1\n"
                "f 1 2 3\n"
                "f 1 2 3 4 5 6 3"; // повторяющиеся индексы
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidPolygon05()
{
    // указано не число в индексах текстурных точек
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "vt 0 0\n"
                "vt 0 1\n"
                "vt 1 1\n"
                "f 1/1 2/2 3/babaka";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidPolygon06()
{
    // указан индекс несуществующей точки
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "vt 0 0\n"
                "vt 0 1\n"
                "vt 1 1\n"
                "f 1/1 2/4 3/3";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidVertex02()
{
    // некорректное задание текстурной точки
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "vt     0  & %* $ ()@  \n"
                "vt 0 1\n"
                "vt 1 1\n"
                "f 1/1 2/2 3/3";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
void ModelLoaderTests::testInvalidPolygon07()
{
    // указаны индексы неопределенных точек
    QString s = "v 1 2 3\n"
                "v -1 -2 -3\n"
                "v 0 0 0\n"
                "vt 0 0\n"
                "vt 1 1\n"
                "vt 1 0\n"
                "f 3/1 2/2 1/3\n"
                "f 1/2 2/1 3/2\n" // повторяющиеся индексы
                "f 1/3 2/2 3/1";
    Scene scene;
    QTextStream stream(&s);
    QString errorStr = loadModel(stream,scene);
    qDebug() << errorStr;

    QVERIFY(!errorStr.isEmpty());
}
