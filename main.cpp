#include "mainwindow.h"
#include <QApplication>

//int raw_data[2];
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();



    return a.exec();
}
