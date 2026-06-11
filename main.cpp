#include "mainwindow.h"
#include <QApplication>

// ISPRAVKA: uklonjen wiringPiSetup() jer wiringPiI2CSetup()
// ne zahtijeva prethodni poziv wiringPiSetup()

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
