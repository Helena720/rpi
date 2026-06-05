#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //postavi QLCDNumber na 5 cifara
    ui->lcdNumber->setDigitCount(5);

    tajmer = new QTimer(this);
    connect(tajmer, SIGNAL(timeout()),
            this, SLOT(ocitajTemperaturu()));
    tajmer->start(1000); //svake sekunde
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ocitajTemperaturu(){
    //DS18B20 cuva podatke ovdje
    QString putanja = "/sys/bus/w1/devices/";
    QDir dir(putanja);

    //nadji folder senzora, on pocinje sa 28-
    QStringList liste = dir.entryList(
        QStringList() << "28-*", QDir::Dirs);
    if(liste.isEmpty()){
        ui->lcdNumber->display("Err");
        return;
    }

    //otvori fajl sa temperaturom
    QString fajl = putanja + liste.first() + "/w1_slave";
    QFile f(fajl);

    if(!f.open(QIODevice::ReadOnly)){
        ui->lcdNumber->display("Err");
        return;
    }
    QTextStream tok(&f);
    QString sadrzaj = tok.readAll();
    f.close();

    //provjera da li je ocitavanje uspjelo
    if(!sadrzaj.contains("YES")){
        ui->lcdNumber->display("Err");
        return;
    }

    //izvuci temperaturu (t=XXXXX)
    int index = sadrzaj.indexOf("t=");
    if(index == -1){
        ui->lcdNumber->display("Err");
        return;
    }

    QString tempStr = sadrzaj.mid(index + 2).trimmed();
    double temp = tempStr.toDouble() / 1000.0; //pretvori u C

    ui->lcdNumber->display(temp);
}
