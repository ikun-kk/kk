#include "widget.h"
#include "ui_widget.h"
#include  "second.h"
#include "QSerialPort"
#include "QSerialPortInfo"
#include "QMessageBox"
#include <QScreen>
#include <weather.h>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{        ui->setupUi(this);
         setAttribute(Qt::WA_StyledBackground);
//给窗口添加背景图
//         QPixmap Images(":/tup/2.png");
//         QPalette Palette = this->palette();
//         Palette.setBrush(QPalette::Background, Images);
//         this->setPalette(Palette);
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

Widget::~Widget()
{
    delete ui;
}



void Widget::on_pushButton_clicked()
{   //切换界面
    this->close();
    second *pic=new second();
     pic->show();
}
void Widget::on_pushButton_4_clicked()
{
    this->close();
    Weather *pic=new Weather();
    pic->show();
}


void Widget::on_pushButton_2_clicked(bool checked)
{  if(checked)
   { //设置要打开的串口
    serialPort->setPortName(ui->comboBox->currentText());
    //设置波特率
//    serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
    serialPort->setBaudRate(115200);
    //设置停止位
//  serialPort->setStopBits(QSerialPort::StopBits(ui->comboBox_4->currentText().toInt()));
    serialPort->setStopBits(QSerialPort::StopBits(1));
    //设置数据位
//    serialPort->setDataBits(QSerialPort::DataBits(ui->comboBox_3->currentText().toInt()));
    serialPort->setDataBits(QSerialPort::DataBits(8));
    //校验位
    switch (/*ui->comboBox_5->currentIndex()*/0) {
    case 0:
        serialPort->setParity(QSerialPort::NoParity);
        break;
//    case 1:
//        serialPort->setParity(QSerialPort::EvenParity);
//        break;
//    case 2:
//        serialPort->setParity(QSerialPort::OddParity);
//        break;
//    case 3:
//        serialPort->setParity(QSerialPort::SpaceParity);
//        break;
//    case 4:
//        serialPort->setParity(QSerialPort::MarkParity);
//        break;
    default:
        break;
    }
    //设置流控
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

     if (!serialPort->open(QIODevice::ReadWrite))
     {  QMessageBox::about(this,"错误","串口打开失败可能被占用了");
        return;
     }
     ui->comboBox->setEnabled(false);

     ui->pushButton_2->setText("关闭串口");

  } else
   {//关闭串口
       serialPort->close();
       ui->comboBox->setEnabled(true);
       ui->pushButton_2->setText("开始获取数据");
   }


}


void Widget::Read_Data()
{
   int t = 0;
   int i;
   QByteArray buf = serialPort->readAll();
   if(!buf.isEmpty())
   {

       if(buf[t] == '#')
       {
           t++;
       if(buf[t] == 'J')
       {
           t++;
           for(i=0; i<6; i++)
           {
               buf3 +=buf[t];
               t++;
               if(buf[t] == 'M')
               {
                 t++;
                 break;
               }
           }

       }
       if(buf[t++] == 'K')
       {
           for(i=0; i<6; i++)
           {
               buf1 +=buf[t];
               t++;
               if(buf[t] == 'M')
               {
                 t++;
                 break;
               }
           }
       }

}
        int32_t hexData1 = buf3.toInt();
        if(hexData1 != 0)
        ui->label_4->setNum(hexData1);

        int32_t  hexData2 = buf1.toInt();
        if(hexData2 != 0)
        ui->label_5->setNum(hexData2);
   }
        buf.clear();
        buf3=0;
        buf1=0;
}


void Widget::on_pushButton_3_clicked()
{
        QByteArray btn_data;
        btn_data[0] = 0x00;
        btn_data[1] = 0x0d;
        btn_data[2] = 0x0a;
        serialPort->write(btn_data);
        if(false == flag_LED0)
         {
            ui->pushButton_3->setText("继电器关闭");
            flag_LED0 = true;
         }
         else
         {
            ui->pushButton_3->setText("继电器打开");
            flag_LED0= false;
         }

}

