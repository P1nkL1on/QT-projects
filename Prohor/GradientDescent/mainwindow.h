#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include "QKeyEvent"
#include "QMouseEvent"

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
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
