#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "field.h"
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    const int fX = 80;
    const int fY = 60;
    const int vero = 2000;
    const int scale = 10;

    Field f = Field(fX, fY, vero);
protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
