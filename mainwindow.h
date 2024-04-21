#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSerialPort>//
#include <QSerialPortInfo>//串口信息类
#include "weather.h"
namespace Ui {
class mainwindow;
}

class mainwindow : public QWidget
{
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private slots:
    void on_pushButton_1_clicked(bool checked);
    void Read_Data();
    void on_pushButton_2_clicked(bool status);

    void on_pushButton_3_clicked(bool status);

    void on_pushButton_clicked();

private:
    Ui::mainwindow *ui;
    QSerialPort *serialPort;
    bool relay;bool led;
    QByteArray buf1,buf2,buf3,buf4,buf5;
    QByteArray data;
};

#endif // MAINWINDOW_H
