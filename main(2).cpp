#include "newwindow.h"
#include <QApplication>
#include <wiringPi.h>
#include <stdio.h>
#include <lcd.h>

int main(int argc, char *argv[])
{
    wiringPiSetup();

    QApplication a(argc, argv);
    NewWindow w;
    w.show();

    return a.exec();
}
