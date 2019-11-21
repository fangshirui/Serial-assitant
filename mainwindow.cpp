//mainwindow.cpp

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/core.hpp>
#include <QDebug>
#include <QStringList>
#include <QProcess>


//using namespace  cv;

double raw_data[2];
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    socket =
    socket = new QUdpSocket();
    socket->bind(QHostAddress::AnyIPv4,8080);
    connect(socket , &QUdpSocket::readyRead, this,&MainWindow::read_road_data);


    //连接信号和槽,定时检查串口缓存区并录入数据
    QObject::connect(&loop, &QTimer::timeout, this, &MainWindow::serialPort_readyRead);




    //发送按键失能
    ui->sendButton->setEnabled(false);
    //波特率默认选择下拉第三项：9600
    ui->baudrateBox->setCurrentIndex(3);

    qDebug() <<"主窗口构造函数";
}

MainWindow::~MainWindow()
{
    if(stop == false)
        delete ui;
    qDebug()<<"主窗口析构函数";
}

void MainWindow::extract_data(QByteArray buffer)
{
   QString str_buffer;
   str_buffer.prepend(buffer);  // 转化为字符串
   str_buffer = str_buffer.mid(1 , str_buffer.size() -2);  // 去除头尾
   QStringList datalist = str_buffer.split(',');  //字符串分割
   raw_data[0] = datalist.at(0).toDouble();
   raw_data[1] = datalist.at(1).toDouble();

   // 添加数据源数量
   raw_data[2] = datalist.at(2).toDouble();
   raw_data[3] = datalist.at(3).toDouble();
}


void MainWindow::serialPort_readyRead()
{

    while(serial.bytesAvailable())
    {
        serial.read(&buffer_temp,1);
        if(buffer_temp == '$'){
            num = 0;
            startbit = true;
        }
        if(startbit == true){
            buffer.append(buffer_temp);
            num++;
        }
        if(startbit == true && buffer_temp == '#'){
            startbit = false;
            newdata = true;
        }
        if(num >40){
            num = 0;
            startbit = 0;
            buffer.clear();
            newdata = false;
        }

        if(newdata)
        {
            extract_data(buffer);
//        qDebug() << raw_data[0] << " " <<raw_data[1];
            ui->recvTextEdit->append(buffer);
            newdata = false;
            buffer.clear();
        }
    }
}


void MainWindow::on_searchButton_clicked()
{
    ui->portNameBox->clear();
    //通过QSerialPortInfo查找可用串口
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->portNameBox->addItem(info.portName());
    }
}

void MainWindow::on_openButton_clicked()
{
    if(ui->openButton->text()==QString("打开串口"))
    {
        //设置串口名
        serial.setPortName(ui->portNameBox->currentText());
        //设置波特率
        serial.setBaudRate(ui->baudrateBox->currentText().toInt());
        //设置数据位数
        switch(ui->dataBitsBox->currentIndex())
        {
        case 8: serial.setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial.setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->stopBitsBox->currentIndex())
        {
        case 1: serial.setStopBits(QSerialPort::OneStop); break;
        case 2: serial.setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial.setFlowControl(QSerialPort::NoFlowControl);

        //打开串口
        if(!serial.open(QIODevice::ReadWrite))
        {
            QMessageBox::about(nullptr, "错误", "无法打开串口");
            return;

        }

        //定时器开始工作
        loop.start(2);

        //下拉菜单控件失能
        ui->portNameBox->setEnabled(false);
        ui->baudrateBox->setEnabled(false);
        ui->dataBitsBox->setEnabled(false);
        ui->ParityBox->setEnabled(false);
        ui->stopBitsBox->setEnabled(false);

        ui->openButton->setText(QString("关闭串口"));
        //发送按键使能
        ui->sendButton->setEnabled(true);
    }
    else
    {
        //关闭定时器
        loop.stop();
        //关闭串口
        serial.close();

        //下拉菜单控件使能
        ui->portNameBox->setEnabled(true);
        ui->baudrateBox->setEnabled(true);
        ui->dataBitsBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->stopBitsBox->setEnabled(true);

        ui->openButton->setText(QString("打开串口"));
        //发送按键失能
        ui->sendButton->setEnabled(false);
    }



}

void MainWindow::on_sendButton_clicked()
{
    //获取界面上的数据并转换成utf8格式的字节流
    QByteArray data = ui->sendTextEdit->toPlainText().toUtf8();
    serial.write(data);
}

void MainWindow::on_clearButton_clicked()
{
    ui->recvTextEdit->clear();
}

// opencv部分
void MainWindow::on_open_cam_Button_clicked()
{
//        ui->open_cam_Button->setEnabled(false);
//        ui->close_cam_Button->setEnabled(true);
//        cv::VideoCapture cap(0);
//        cv::Mat frame;
//        cv::Mat newframe;
//        stop = false;
//        cv::namedWindow("当前图像",cv::WINDOW_AUTOSIZE);
//        cv::namedWindow("高斯平滑",cv::WINDOW_AUTOSIZE);
//        while(!stop	)
//        {
//            cap >> frame;
//            cv::cvtColor(frame,frame,cv::COLOR_BGR2GRAY);
//            cv::imshow("当前图像", frame);

//            cv::pyrDown(frame, newframe);
//            cv::pyrDown(newframe, newframe);
//            cv::Canny(newframe,newframe,30,80);

////            cv::GaussianBlur(newframe, newframe , cv::Size(5,5),3,3);
//            cv::imshow("高斯平滑",newframe);
//            cv::waitKey(20);
//        }

//        cap.release();
//        cv::destroyAllWindows();
    road_dect = new QProcess(this);

    //连接外部进程与主进程的相关问题

    road_dect->start("/home/kk/study/QT/serial/test.py");

    qDebug()<<"打开摄像头窗口";



}

void MainWindow::on_close_cam_Button_clicked()
{

}

void MainWindow::on_open_monitor_clicked()
{
    monitor = new Dialog(this);  // 虽然是用new 创建， 但由于是从父窗口派生的，所以在程序结束时会自动调用析构函数
    monitor->show();
}


void MainWindow::on_close_monitor_clicked()
{
    monitor->close();

}

void MainWindow::read_road_data()
{
    QByteArray road_data;
    road_data.resize(socket->bytesAvailable());
    socket->readDatagram(road_data.data(),road_data.size());
    state = road_data.toInt();
    qDebug() << state;

//    qDebug()<< road_str;
}


