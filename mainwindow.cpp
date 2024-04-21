#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include "QMessageBox"
#include <QDebug>
mainwindow::mainwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwindow)
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint); // 无边框
    setFixedSize(800,480);// 固定窗口大小
    QList <QScreen *> list_screen =  QGuiApplication::screens();
#if __arm__
/* 重设大小 */
this->resize(list_screen.at(0)->geometry().width(),
          list_screen.at(0)->geometry().height());
#else
/* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif

    serialPort=new QSerialPort(this);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());//查找可用串口。
    }
//读取数据
connect(serialPort,SIGNAL(readyRead()),this,SLOT(Read_Data()));

}

mainwindow::~mainwindow()
{
    delete ui;
}




void mainwindow::on_pushButton_1_clicked(bool checked)
{
        if(checked)
      { serialPort->setPortName(ui->comboBox->currentText()); //设置要打开的串口
        serialPort->setBaudRate(115200);//设置波特率
        serialPort->setStopBits(QSerialPort::StopBits(1));//设置停止位
        serialPort->setDataBits(QSerialPort::DataBits(8));//设置数据位
        serialPort->setParity(QSerialPort::NoParity);//校验位
        serialPort->setFlowControl(QSerialPort::NoFlowControl);//设置流控

         if (!serialPort->open(QIODevice::ReadWrite))
         {  QMessageBox::about(this,"错误","串口打开失败可能被占用了");
            return;
         }
         ui->comboBox->setEnabled(false);

         ui->pushButton_1->setText("关闭串口");

        }
        else
         {//关闭串口
           serialPort->close();
           ui->comboBox->setEnabled(true);
           ui->pushButton_1->setText("打开串口");
        }
}

void mainwindow::Read_Data()
{
    int t = 0;
    int i;
    QByteArray buf = serialPort->readAll();
    qDebug()<<buf;
    if(!buf.isEmpty())
    {

        if(buf[t] == 'A')
        {
            t++;
        if(buf[t] == 'B')
        {
            t++;
            for(i=0; i<6; i++)
            {
                buf1 +=buf[t];
                t++;
                if(buf[t] == 'C')
                {
                  t++;
                  break;
                }
            }

        }
        if(buf[t++] == 'D')
        {
            for(i=0; i<6; i++)
            {
                buf2 +=buf[t];
                t++;
                if(buf[t] == 'E')
                {
                  t++;
                  break;
                }
            }
        }
        if(buf[t++] == 'F')
        {
            for(i=0; i<6; i++)
            {
                buf3 +=buf[t];
                t++;
                if(buf[t] == 'G')
                {
                  t++;
                  break;
                }
            }
        }
        if(buf[t++] == 'H')
        {
            for(i=0; i<6; i++)
            {
                buf4 +=buf[t];
                t++;
                if(buf[t] == 'I')
                {
                  t++;
                  break;
                }
            }
        }
        if(buf[t++] == 'J')
        {
            for(i=0; i<6; i++)
            {
                buf5 +=buf[t];
                t++;
                if(buf[t] == 'K')
                {
                  t++;
                  break;
                }
            }
        }



 }
         int32_t hexData1 = buf1.toInt();
         if(hexData1 != 0)
         ui->label_5->setNum(hexData1);

         int32_t  hexData2 = buf2.toInt();
         if(hexData2 != 0)
         ui->label_10->setNum(hexData2);

         int32_t  hexData3 = buf3.toInt();
         if(hexData3 != 0||hexData3 ==0)
         ui->label_13->setNum(hexData3);

         int32_t  hexData4 = buf5.toInt();
         if(hexData4 != 0)
         ui->label_17->setNum(hexData4);

         int32_t  hexData5 = buf4.toInt();
         if(hexData5 != 0||hexData5 ==0)
         ui->label_21->setNum(hexData5);
 }
         buf.clear();

        buf1=0; buf2=0;buf3=0;buf4=0;buf5=0;
}



void mainwindow::on_pushButton_2_clicked(bool status)
{
    QByteArray btn_data;
    if(false == relay)
     {
        ui->pushButton_2->setText("关闭水泵");
        relay = true;
        btn_data[0] = 0x00;
        btn_data[1] = 0x0d;
        btn_data[2] = 0x0a;

     }
     else
     {
        ui->pushButton_2->setText("打开水泵");
        relay= false;
        btn_data[0] = 0x01;
        btn_data[1] = 0x0d;
        btn_data[2] = 0x0a;
     }
          qDebug()<<"值："<<btn_data;
          serialPort->write(btn_data);
}

void mainwindow::on_pushButton_3_clicked(bool status)
{
    QByteArray btn_data;

    if(false == led)
     {
        ui->pushButton_3->setText("关闭补光");
        led = true;
        btn_data[0] = 0x02;
        btn_data[1] = 0x0d;
        btn_data[2] = 0x0a;

     }
     else
     {
        ui->pushButton_3->setText("打开补光");
        led= false;
        btn_data[0] = 0x03;
        btn_data[1] = 0x0d;
        btn_data[2] = 0x0a;
     }
        serialPort->write(btn_data);
}

void mainwindow::on_pushButton_clicked()
{
    this->close();
    Weather *pic=new Weather();
     pic->show();
}
