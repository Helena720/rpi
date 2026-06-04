#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define I2C_ADRESA 0x48

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ocitajSenzore();

private:
    Ui::MainWindow *ui;
    QTimer *tajmer;
    int i2c_fd;
};

#endif // MAINWINDOW_H
