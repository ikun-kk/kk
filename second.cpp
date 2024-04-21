#include "second.h"
#include "ui_second.h"
#include "widget.h"
#include "ap3216c.h"

#include <QScreen>

second::second(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::second)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    ap3216c = new Ap3216c(this);
    connect(ap3216c,SIGNAL(ap3216cDataChanged()),this,SLOT(handleResults()));
    workerThread = new WorkerThread(this);
    connect(workerThread, SIGNAL(b()),
                this, SLOT(qiehuan()));
//    workerThread->start();
//    workerThread3 = new WorkerThread3(this);
//    connect(workerThread3, SIGNAL(c()),
//                this, SLOT(qiehuan()));
//        workerThread3->start();
    QList <QScreen *> list_screen =  QGuiApplication::screens();

#if __arm__
     ap3216c->setCapture(true);
    /* 重设大小 */
     this->resize(list_screen.at(0)->geometry().width(),
                  list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif

}

second::~second()
{
    delete ui;
    workerThread->quit();
}

void second::on_pushButton_clicked()
{  workerThread->start();
//    Widget *pic=new Widget();
//     pic->show();
//       this->close();
}
void second::handleResults()
{
        static QString als = ap3216c->alsData();
        if (als != ap3216c->alsData()) {
            als = ap3216c->alsData(); ui->label_3->setText(als);
        }
        static QString ps = ap3216c->psData();
            if (ps != ap3216c->psData()) {
                ps = ap3216c->psData(); ui->label_6->setText(ps);
            }
          static QString ir = ap3216c->irData();
                if (ir != ap3216c->irData()) {
                   ir = ap3216c->irData(); ui->label_7->setText(ir);
                }

}

void second::qiehuan()
{
    Widget *pic=new Widget();
     pic->show();
     this->close();
}


