#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void drawCanvas(const QVector<QVector2D> resPoints, const QVector<unsigned int> vertIndexes,
                    const QVector<unsigned int> polIndStart, const int screenWidth, const int screenHeight);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *e);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
