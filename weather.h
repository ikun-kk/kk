#ifndef WEATHER_H
#define WEATHER_H
#pragma execution_character_set("utf-8")
#include <QWidget>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include<QLabel>
#include<QPainter>
#include <QComboBox>
#include "weatherdata.h"
#include "mainwindow.h"
//不能再次包含头文件，weather.h多次包含，weathertool.h也会多次包含
//#include "weathertool.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Weather; }
QT_END_NAMESPACE

class Weather : public QWidget
{
    Q_OBJECT

public:
    Weather(QWidget *parent = nullptr);
    ~Weather();

private:
    Ui::Weather *ui;


protected:
    void contextMenuEvent(QContextMenuEvent *e);
    void mousePressEvent(QMouseEvent *e);   // 鼠标点击事件
    void mouseMoveEvent(QMouseEvent *e);    // 鼠标移动事件
    void getWeatherInfo(QString cityName);  // 通过城市编码获取天气信息
    void parseJson(QByteArray& byteArray);  // 分析Json数据
    void updateUI();  // 更新UI
    // 重写父类的eventFilter方法
    bool eventFilter(QObject* watched,QEvent *e);
    void paintHighCurve();//绘制高温曲线
    void paintLowCurve();//绘制低温曲线

private:
    QMenu *mExitMenu;// 右键菜单
    QAction *mExitAct;// 退出的行为-右键菜单项

    QPoint mOffset;// 窗口移动时，鼠标与窗口左上角的偏移

    QNetworkAccessManager *mNetAccessManager;// Http请求

    Today mToday;// 今天
    Day mDay[6];// 6天数据

    QList<QLabel*> mWeekList;   // 星期
    QList<QLabel*> mDateList;   // 日期

    QList<QLabel*> mTypeList;   // 天气类型
    QList<QLabel*> mTypeIconList;   // 天气类型图标

    QList<QLabel*> mAqiList;   // 天气污染指数

    QList<QLabel*> mFxList;   // 风向
    QList<QLabel*> mFlList;   // 风力

    QMap<QString,QString> mTypeMap;// 天气类型图标路径
  QComboBox *comboBox;
private slots:
    void onReplied(QNetworkReply *reply);// 处理HTTP服务返回数据

    void on_btnSearch_4_clicked();
    void on_pushButton_clicked();
};
#endif // WEATHER_H
