#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QDialog>
#include <QPixmap>

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
    void on_pushButton_clicked();

private:
    Ui::NewWindow *ui;
};

#endif // NEWWINDOW_H
