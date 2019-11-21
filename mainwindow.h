//mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <dialog.h>
#include <QTimer>
#include <QProcess>
#include <QUdpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void extract_data(QByteArray buffer);


private slots:
    void serialPort_readyRead();

    void on_searchButton_clicked();

    void on_openButton_clicked();

    void on_sendButton_clicked();

    void on_clearButton_clicked();

    void on_open_cam_Button_clicked();

    void on_close_cam_Button_clicked();

    void on_open_monitor_clicked();



    void on_close_monitor_clicked();

    //socket通信相关
    void read_road_data();


private:
    Ui::MainWindow *ui;
    QProcess * road_dect;


    bool stop = false;  // 和opencv 的打开关闭有关的标志位


    Dialog  * monitor = nullptr;   // 监控器画面



    QSerialPort serial;  //串口类
    QByteArray buffer;   // 储存一条数据
    char buffer_temp;     //储存一个字节
    int num;
    bool startbit = false;      // 判断是否开始写新数据
    bool newdata = false;        //判断是否新数据可用
    QTimer loop;      //定时器循环运行，用于定时检查串口的缓存区

    int state = 1;  // 0 1 2道路状态检测位

    // socket
    QUdpSocket* socket;


};
#endif // MAINWINDOW_H
