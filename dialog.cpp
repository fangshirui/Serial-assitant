#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <sys/stat.h>
//#include <stdio.h>
#include <unistd.h>
extern double raw_data[];
Dialog::Dialog(QWidget *parent) :
//    QDialog(parent, Qt::WindowTitleHint | Qt::CustomizeWindowHint),   // 隐藏关闭按钮，因为其不可用
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(&dataTimer , SIGNAL(timeout()) , this , SLOT(realtimeDataSlot()));

    // 设置图形可缩放
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);


//    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//    timeTicker->setTimeFormat("%h:%m:%s");
//    ui->plot->xAxis->setTicks(timeTicker);
//    ui->plot->axisRect()->setupFullAxesBox();

    ui->plot->yAxis2->setVisible(true);
    // 设置轴的范围
    ui->plot->yAxis->setRange(-100,100);
    ui->plot->yAxis2->setRange(0,1);


    qDebug() << "monitor构造函数";

}

Dialog::~Dialog()
{
    delete ui;
    qDebug()<< "monitor析构函数";
}

void Dialog::on_btn_draw_clicked()
{


    ui->plot->addGraph();
    ui->plot->graph(0)->setPen(QPen(QColor(40,110,255)));
    ui->plot->addGraph();
    ui->plot->graph(1)->setPen(QPen(QColor(255,110,40)));


    //滑移率图形
    ui->plot->addGraph(ui->plot->xAxis,ui->plot->yAxis2);
    ui->plot->graph(2)->setPen(QColor(Qt::green));

    // 添加数据图形（平衡车项目多达四个数据）
    ui->plot->addGraph();
    ui->plot->graph(3)->setPen(QPen(QColor(34,110,40)));
    ui->plot->addGraph();
    ui->plot->graph(4)->setPen(QPen(QColor(255,15,60)));

    // 定时器越快越好,定时器相当于中断触发
    dataTimer.start(5);  // 括号内是信号发送周期 ， 0ms 最快模式

    time.start();  //横坐标计时开始
}

void Dialog::realtimeDataSlot()
{

    double key = time.elapsed() / 1000.0;  // time.elapsed() 返回的是开始初始化的时间到目前时间共有多少毫秒
    if(key - lastPointKey >= 0.01 ){   // 每隔10毫秒读取一次缓存的数据 raw_data
        ui->plot->graph(0)->addData(key ,raw_data[0]);
//        qDebug() << raw_data[0] << " " <<raw_data[1];
        ui->plot->graph(1)->addData(key,raw_data[1]);

        //多余数据源
        ui->plot->graph(3)->addData(key,raw_data[2]);
        ui->plot->graph(4)->addData(key,raw_data[3]);

        // 滑移率图形
//        double slip = raw_data[0]/raw_data[1];
//        ui->plot->graph(2)->addData(key,slip);
        lastPointKey = key;
    }

    ui->plot->xAxis->setRange(key , 8 , Qt::AlignRight);  // 始终靠右侧
    ui->plot->replot();


}

void Dialog::on_btn_pause_clicked()
{
    dataTimer.stop();
    ui->btn_draw->setEnabled(false);
}

void Dialog::on_btn_clear_clicked()
{
    dataTimer.stop();
    ui->plot->clearPlottables();  //删除了所有曲线数据


    lastPointKey = 0;
    ui->plot->replot();
    ui->btn_draw->setEnabled(true);

}




void Dialog::on_btn_save_clicked()
{
    FILE * fp = 0;
    if ( (fp = fopen("/home/kk/study/QT/serial/data.txt","w")) == 0 ){
        qDebug() << "文件打开错误";
    }
    fprintf(fp , " afsdsdfsdfasdfsafsdfsdfjslfjsdlfsdjl");

    // 刷新文件 ，（不会自动将缓冲区写入文件的，一般在结束时自动写入文件）
    fflush(fp);


    fclose(fp);
}

void Dialog::on_btn_auto_adjust_clicked()
{
    ui->plot->yAxis->setRange(0,100);
    ui->plot->replot();
}
