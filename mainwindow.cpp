#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Samo prikaz - korisnik ne moze da pomjera
    ui->dial->setDisabled(true);
    ui->verticalSlider->setDisabled(true);

    // Opseg 0-255 (8-bitni ADC)
    ui->dial->setRange(0, 255);
    ui->verticalSlider->setRange(0, 255);

    // I2C inicijalizacija
    i2c_fd = wiringPiI2CSetup(I2C_ADRESA);

    // Tajmer - ocitava senzore jednom u sekundi
    tajmer = new QTimer(this);
    connect(tajmer, SIGNAL(timeout()), this, SLOT(ocitajSenzore()));
    tajmer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ocitajSenzore()
{
    int vrednosti[3];
    // Kanal 0 - osvetljaj
    // Kanal 1 - temperatura (NTC)
    // Kanal 2 - trimer potenciometar

    for (int i = 0; i < 3; i++) {
        wiringPiI2CWrite(i2c_fd, 0x40 | i); // kontrolni bajt + kanal
        wiringPiI2CRead(i2c_fd);             // odbaci prvo citanje (stara vrijednost)
        vrednosti[i] = wiringPiI2CRead(i2c_fd); // stvarna vrijednost
    }

    // Osvetljaj -> vertikalni slider (invertovano: vise svjetla = vise na slideru)
    ui->verticalSlider->setValue(255 - vrednosti[0]);

    // Temperatura -> QLCDNumber (skaliranje 0-255 na 0-100)
    ui->lcdNumber->display(vrednosti[1] / 255.0 * 100);

    // Trimer potenciometar -> dial
    ui->dial->setValue(vrednosti[2]);
}
