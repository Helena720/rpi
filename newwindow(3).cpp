#include "newwindow.h"
#include "ui_newwindow.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <QMessageBox>
#include <QTime>

extern int rtc_fd;

// konverzija BCD → decimalni
static int bcd2dec(int bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

// konverzija decimalni → BCD
static int dec2bcd(int dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

NewWindow::NewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWindow)
{
    ui->setupUi(this);

    // kreiranje tajmera koji osvežava prikaz svakih 1 sekundu
    tajmer = new QTimer(this);
    connect(tajmer, SIGNAL(timeout()), this, SLOT(azurirajVreme()));
    tajmer->start(1000);
}

NewWindow::~NewWindow()
{
    delete ui;
}

void NewWindow::azurirajVreme()
{
    // čitanje trenutnog vremena sa PCF8563
    int sek = bcd2dec(wiringPiI2CReadReg8(rtc_fd, 0x02) & 0x7F);
    int min = bcd2dec(wiringPiI2CReadReg8(rtc_fd, 0x03) & 0x7F);
    int sat = bcd2dec(wiringPiI2CReadReg8(rtc_fd, 0x04) & 0x3F);

    // ažuriranje labele sa trenutnim vremenom
    QTime trenutno(sat, min, sek);
    ui->label_vreme->setText(trenutno.toString("hh:mm:ss"));
}

void NewWindow::on_pushButton_potvrdi_clicked()
{
    QString unos = ui->lineEdit_vreme->text();
    QTime novoVreme = QTime::fromString(unos, "hh:mm:ss");

    if (!novoVreme.isValid()) {
        QMessageBox::warning(this, "Greška", "Format mora biti hh:mm:ss");
        return;
    }

    wiringPiI2CWriteReg8(rtc_fd, 0x00, 0x20);

    wiringPiI2CWriteReg8(rtc_fd, 0x02, dec2bcd(novoVreme.second()));
    wiringPiI2CWriteReg8(rtc_fd, 0x03, dec2bcd(novoVreme.minute()));
    wiringPiI2CWriteReg8(rtc_fd, 0x04, dec2bcd(novoVreme.hour()));

    wiringPiI2CWriteReg8(rtc_fd, 0x00, 0x00);

    QMessageBox::information(this, "Uspeh", "Vreme u RTC modulu je ažurirano!");
}
