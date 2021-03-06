#ifndef MODELLOADERTESTS_H
#define MODELLOADERTESTS_H

#include <QObject>
#include <QTest>

class ModelLoaderTests : public QObject
{
    Q_OBJECT
public:
    explicit ModelLoaderTests(QObject *parent = 0);
private slots:
    void testNormalModel01();
    void testEmptyString01();
    void testInvalidVertex01();
    void testInvalidVertex02();
    void testInvalidPolygon01();
    void testInvalidPolygon02();
    void testInvalidPolygon03();
    void testInvalidPolygon04();
    void testInvalidPolygon05();
    void testInvalidPolygon06();
    void testInvalidPolygon07();

};

#endif // MODELLOADERTESTS_H
