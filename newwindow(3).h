#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class NewWindow;
}

class NewWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NewWindow(QWidget *parent = 0);
    ~NewWindow();

private slots:
    void on_pushButton_potvrdi_clicked();
    void azurirajVreme();

private:
    Ui::NewWindow *ui;
    QTimer        *tajmer;
};

#endif // NEWWINDOW_H
