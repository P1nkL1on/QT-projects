#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>

#include "QMouseEvent"
#include "QKeyEvent"

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
    void mouseMoveEvent(QMouseEvent *m);
    void mousePressEvent(QMouseEvent *m);
    void mouseReleaseEvent(QMouseEvent *m);

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
