#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>//
#include <QSerialPortInfo>//串口信息类
#include <QThread>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked(bool checked);
    void Read_Data();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;
    QByteArray buf1,buf2,buf3;
    bool flag_LED0;
};

#endif // WIDGET_H
