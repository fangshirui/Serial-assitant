#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTime>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btn_draw_clicked();
    void realtimeDataSlot();

    void on_btn_pause_clicked();

    void on_btn_clear_clicked();



    void on_btn_save_clicked();

    void on_btn_auto_adjust_clicked();

private:
    Ui::Dialog *ui;
    QTimer dataTimer;
    QTime  time;


    double lastPointKey = 0;
};

#endif // DIALOG_H
