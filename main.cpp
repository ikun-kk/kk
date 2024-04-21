#include "widget.h"

#include <QApplication>
#include "second.h"
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Widget w;
//    w.show();
//  second *pic=new second();
//  pic->show();
    mainwindow *pic=new mainwindow();
    pic->show();
    return a.exec();
}
