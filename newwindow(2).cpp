#include "newwindow.h"
#include "ui_newwindow.h"
#include <wiringPi.h>
#include <lcd.h>
#include <string.h>

// Pinovi za LCD (isti kao u balija projektu)
const int RS = 3;
const int EN = 14;
const int D0 = 4;
const int D1 = 12;
const int D2 = 13;
const int D3 = 6;

int lcd_h;

NewWindow::NewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewWindow)
{
    ui->setupUi(this);

    // Ograniciti unos na 16 karaktera po redu
    ui->lineEdit->setMaxLength(16);
    ui->lineEdit_2->setMaxLength(16);

    // Inicijalizacija LCD-a: 2 reda, 16 kolona, 4-bitni mod
    lcd_h = lcdInit(2, 16, 4, RS, EN, D0, D1, D2, D3, 0, 0, 0, 0);
    lcdClear(lcd_h);
}

NewWindow::~NewWindow()
{
    delete ui;
}

void NewWindow::on_pushButton_clicked()
{
    QString red1 = ui->lineEdit->text();
    QString red2 = ui->lineEdit_2->text();

    lcdClear(lcd_h);

    // Ispis prvog reda na LCD
    lcdPosition(lcd_h, 0, 0);
    lcdPrintf(lcd_h, red1.toStdString().c_str());

    // Ispis drugog reda na LCD
    lcdPosition(lcd_h, 0, 1);
    lcdPrintf(lcd_h, red2.toStdString().c_str());
}
