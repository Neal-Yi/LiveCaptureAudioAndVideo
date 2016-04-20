#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <stdio.h>
int main(int argc, char *argv[])
{
    //freopen("log.txt","wb",stdout);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

}
