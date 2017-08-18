#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "QKeyEvent"
#include <QMainWindow>

#include "QTimer"
#include "QTime"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent * event);
    QTimer* worldTimer;

    int nowLong = 10;
private slots:
    void updateWorldByTimer();


};

#endif // MAINWINDOW_H
