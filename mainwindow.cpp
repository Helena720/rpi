#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <wiringPiI2C.h>
#include <QApplication>

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Merenje napona - YL-40");

    // I2C inicijalizacija - PCF8591 na YL-40
    i2c_fd = wiringPiI2CSetup(I2C_ADRESA);

    sekunda = 0;

    // Kreiranje serije podataka (linija na grafiku)
    series = new QLineSeries();

    // Kreiranje grafika
    chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Merenje napona");

    // X osa - Vreme (0-5 sekundi)
    axisX = new QValueAxis();
    axisX->setRange(0, 5);
    axisX->setTickCount(6);   // oznake: 0,1,2,3,4,5
    axisX->setTitleText("Vreme");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y osa - Napon (0-3.3V)
    axisY = new QValueAxis();
    axisY->setRange(0, 3.3);
    axisY->setTitleText("Napon");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Kreiranje prikaza grafika
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Postavljanje grafika u verticalLayout iz .ui fajla
    ui->verticalLayout->addWidget(chartView);

    // Tajmer - startuje se tek na klik dugmeta Start
    tajmer = new QTimer(this);
    connect(tajmer, SIGNAL(timeout()), this, SLOT(ocitajNapon()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ocitajNapon()
{
    // Citanje kanala 2 (trimer potenciometar) sa PCF8591
    wiringPiI2CWrite(i2c_fd, 0x40 | 2); // kanal 2 = trimer
    wiringPiI2CRead(i2c_fd);             // odbaci prvo citanje
    int raw = wiringPiI2CRead(i2c_fd);   // stvarna vrijednost 0-255

    // Konverzija u volte (0-3.3V)
    double napon = raw / 255.0 * 3.3;

    // Dodavanje tacke na grafik
    series->append(sekunda, napon);

    sekunda++;

    // ISPRAVKA: reset tek nakon sto se doda tacka na sekundi 5
    // Na taj nacin se prikazuju tacke 0,1,2,3,4,5 (ukupno 6 tacaka)
    if (sekunda > 5) {
        series->clear();
        sekunda = 0;
    }
}

void MainWindow::on_pushButton_start_clicked()
{
    series->clear();
    sekunda = 0;
    tajmer->start(1000);
}

void MainWindow::on_pushButton_obrisi_clicked()
{
    tajmer->stop();
    series->clear();
    sekunda = 0;
}

void MainWindow::on_pushButton_zatvori_clicked()
{
    QApplication::quit();
}
