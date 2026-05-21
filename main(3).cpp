#include "newwindow.h"
#include <QApplication>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <stdio.h>

// I2C adresa PCF8563 (fiksna)
#define RTC_ADRESA 0x51

int rtc_fd;

int main(int argc, char *argv[])
{
    wiringPiSetup();

    // inicijalizacija I2C komunikacije sa PCF8563
    rtc_fd = wiringPiI2CSetup(RTC_ADRESA);
    if (rtc_fd < 0) {
        fprintf(stderr, "Greška: I2C inicijalizacija neuspešna!\n");
        return 1;
    }

    // postavljanje normalnog režima rada
    wiringPiI2CWriteReg8(rtc_fd, 0x00, 0x00);

    QApplication a(argc, argv);
    NewWindow w;
    w.show();

    return a.exec();
}
