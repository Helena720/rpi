#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <wiringPiI2C.h>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#define I2C_ADRESA 0x48

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void ocitajNapon();
    void on_pushButton_start_clicked();
    void on_pushButton_obrisi_clicked();
    void on_pushButton_zatvori_clicked();

private:
    Ui::MainWindow *ui;
    QTimer         *tajmer;
    int             i2c_fd;

    QLineSeries    *series;
    QChart         *chart;
    QChartView     *chartView;
    QValueAxis     *axisX;
    QValueAxis     *axisY;

    int sekunda;
};

#endif // MAINWINDOW_H
