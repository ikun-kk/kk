#include "weather.h"
#include "ui_weather.h"
#include<QDebug>
#include<QMessageBox>
#include <QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include "mainwindow.h"
#include <QScreen>
#pragma execution_character_set("utf-8")
#define INCREMENT 3  // 温度每升高/降低 1°，y轴坐标的增量
#define POINT_RADIUS 3 // 曲线描点的大小(点的半径)
#define TEXT_OFFSET_X 12  // 文字偏移
#define TEXT_OFFSET_X 12

Weather::Weather(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Weather)
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
    QPushButton *button = new QPushButton("返回", this);
    button->setGeometry(270, 15, 80, 30); // 设置按钮位置和大小
    comboBox = new QComboBox(this);
    comboBox->addItem("北京");
    comboBox->addItem("上海");
    comboBox->addItem("揭阳");
    comboBox->addItem("韶关");
    comboBox->addItem("深圳");
    comboBox->addItem("汕头");
    comboBox->setGeometry(5, 15, 235, 35);
    connect(button, &QPushButton::clicked, this, &Weather::on_pushButton_clicked);
    {// 右键退出
        mExitMenu=new QMenu(this);
        mExitAct=new QAction();
        mExitAct->setText("退出");
        mExitAct->setIcon(QIcon(":/res/close.png"));
        mExitMenu->addAction(mExitAct);
        connect(mExitAct,&QAction::triggered,this,[=](){
            qApp->exit(0);
        });
    }

    //网络对象
    mNetAccessManager=new QNetworkAccessManager(this);
    connect(mNetAccessManager,SIGNAL(finished(QNetworkReply *)),this,SLOT(onReplied(QNetworkReply *)));

    // 默认获取 北京 天气信息
    getWeatherInfo("北京");

    {// 将控件放到数组里面，方便使用循环进行处理
        // 星期
        mWeekList<<ui->lblWeek0_4<<ui->lblWeek1_4<<ui->lblWeek2_4<<ui->lblWeek3_4<<ui->lblWeek4_4<<ui->lblWeek5_4;
        // 日期
        mDateList<<ui->lblDate0_4<<ui->lblDate1_4<<ui->lblDate2_4<<ui->lblDate3_4<<ui->lblDate4_4<<ui->lblDate5_4;
        // 天气类型
        mTypeList<<ui->lblType0_4<<ui->lblType1_4<<ui->lblType2_4<<ui->lblType3_4<<ui->lblType4_4<<ui->lblType5_4;
        // 天气类型图标
        mTypeIconList<<ui->lblTypeIcon0_4<<ui->lblTypeIcon1_4<<ui->lblTypeIcon2_4<<ui->lblTypeIcon3_4<<ui->lblTypeIcon4_4<<ui->lblTypeIcon5_4;
        // 天气污染指数
        mAqiList<<ui->lblQuality0_4<<ui->lblQuality1_4<<ui->lblQuality2_4<<ui->lblQuality3_4<<ui->lblQuality4_4<<ui->lblQuality5_4;
        // 风向
        mFxList<<ui->lblFx0_4<<ui->lblFx1_4<<ui->lblFx2_4<<ui->lblFx3_4<<ui->lblFx4_4<<ui->lblFx5_4;
        // 风力
        mFlList<<ui->lblFl0_4<<ui->lblFl1_4<<ui->lblFl2_4<<ui->lblFl3_4<<ui->lblFl4_4<<ui->lblFl5_4;

        // 天气类型图标路径
        //根据keys,设置icon的路径
        mTypeMap.insert("暴雪",":/res/type/BaoXue.png");
        mTypeMap.insert("暴雨",":/res/type/BaoYu. png");
        mTypeMap.insert("暴雨到大暴雨",":/res/type/BaoYuDaoDaBaoYu.png");
        mTypeMap.insert("大暴雨",":/res/type/DaBaoYu.png");
        mTypeMap.insert("大暴雨到特大暴雨",":/res/type/DaBaoYuDaoTeDaBaoYu.png");
        mTypeMap.insert("大到暴雪",":/res/type/DaDaoBaoXue.png");
        mTypeMap.insert("大雪",":/res/type/DaXue.png");
        mTypeMap.insert("大雨",":/res/type/DaYu.png");
        mTypeMap.insert("冻雨",":/res/type/DongYu.png");
        mTypeMap.insert("多云",":/res/type/DuoYun.png");
        mTypeMap.insert("浮沉",":/res/type/FuChen.png");
        mTypeMap.insert("雷阵雨",":/res/type/LeiZhenYu.png");
        mTypeMap.insert("雷阵雨伴有冰雹",":/res/type/LeiZhenYuBanYouBingBao.png");
        mTypeMap.insert("霾",":/res/type/Mai.png");
        mTypeMap.insert("强沙尘暴",":/res/type/QiangShaChenBao.png");
        mTypeMap.insert("晴",":/res/type/Qing.png");
        mTypeMap.insert("沙尘暴",":/res/type/ShaChenBao.png");
        mTypeMap.insert("特大暴雨",":/res/type/TeDaBaoYu.png");
        mTypeMap.insert("undefined",":/res/type/undefined.png");
        mTypeMap.insert("雾",":/res/type/Wu.png");
        mTypeMap.insert("小到中雪",":/res/type/XiaoDaoZhongXue.png");
        mTypeMap.insert("小到中雨",":/res/type/XiaoDaoZhongYu.png");
        mTypeMap.insert("小雪",":/res/type/XiaoXue.png");
        mTypeMap.insert("小雨",":/res/type/XiaoYu.png");
        mTypeMap.insert("雪",":/res/type/Xue.png");
        mTypeMap.insert("扬沙",":/res/type/YangSha.png");
        mTypeMap.insert("阴",":/res/type/Yin.png");
        mTypeMap.insert("雨",":/res/type/Yu.png");
        mTypeMap.insert("雨夹雪",":/res/type/YuJiaXue.png");
        mTypeMap.insert("阵雪",":/res/type/ZhenXue.png");
        mTypeMap.insert("阵雨",":/res/type/ZhenYu.png");
        mTypeMap.insert("中到大雪",":/res/type/ZhongDaoDaXue.png");
        mTypeMap.insert("中到大雨",":/res/type/ZhongDaoDaYu.png");
        mTypeMap.insert("中雪",":/res/type/ZhongXue.png");
        mTypeMap.insert("中雨",":/res/type/ZhongYu.png");
    }

    //给标签安装事件过滤器，捕获所有事件
    //参数指定为this，也就是当前窗口对象
    ui->lblHighCurve_4->installEventFilter(this);
    ui->lblLowCurve_4->installEventFilter(this);
}

Weather::~Weather()
{
    delete ui;
}

//重写父类的虚函数
//父类中默认的实现是忽略右键菜单事件
//重写之后，就可以处理右键菜单
void Weather::contextMenuEvent(QContextMenuEvent *e)
{
    // 弹出右键菜单
    mExitMenu->exec(QCursor::pos());

    //调用accept表示，这个事件我已经处理，不需要向上传递了
    e->accept();//被用于接受QContextMenuEvent事件并将其标记为已处理，以便不会再次触发相同的事件
}

// 鼠标点击事件
void Weather::mousePressEvent(QMouseEvent *e)
{
    mOffset=e->globalPos()-this->pos();
}

// 鼠标移动事件
void Weather::mouseMoveEvent(QMouseEvent *e)
{
    this->move(e->globalPos()-mOffset);
}

#include "weathertool.h"
// 在头文件中定义了静态变量，然后在多个源文件中包含了这个头文件，
// 就会出现同一个变量被多次定义的情况，从而导致链接错误,所以放在.cpp中

/*
 * 也可以使用inline内联声明变量，就能解决上面的问题；
 * 注意：使用inline需要在.pro修改CONFIG += c++17
*/

// 通过城市编码获取天气信息
void Weather::getWeatherInfo(QString cityName)
{
    QString cityCode=WeatherTool::getCityCode(cityName);
    QUrl url("http://t.weather.itboy.net/api/weather/city/"+cityCode);
    mNetAccessManager->get(QNetworkRequest(url));
}

// 分析Json数据
void Weather::parseJson(QByteArray &byteArray)
{
    QJsonParseError err;
    QJsonDocument doc=QJsonDocument::fromJson(byteArray,&err);
    if(err.error!=QJsonParseError::NoError)
    {
        qDebug()<<"parse Json error";
        return;
    }

    QJsonObject rootObj=doc.object();// 根下的对象
    qDebug()<<rootObj.value("message").toString();

    //1.解析日期和城市
    mToday.date = rootObj.value("date").toString();
    mToday.city = rootObj.value("cityInfo").toObject().value("city").toString();

    //2.解析yesterday
    QJsonObject objData = rootObj.value("data").toObject();
    QJsonObject objYesterday = objData.value("yesterday").toObject();
    mDay[0].date=objYesterday.value("ymd").toString();
    mDay[0].week=objYesterday.value("week").toString();
    mDay[0].type=objYesterday.value("type").toString();

    qDebug()<<"high :"<<objYesterday.value("high").toString();
    // "high":"高温  18°","low":"低温  6°"
    QString s;
    s=objYesterday.value("high").toString().split(" ").at(1);// 18°
    s=s.left(s.length()-1);// 18
    mDay[0].high=s.toInt();


    s=objYesterday.value("low").toString().split(" ").at(1);// 6°
    s=s.left(s.length()-1);// 6

    mDay[0].low=s.toInt();
    mDay[0].fl = objYesterday.value("fl").toString();// 风力
    mDay[0].fx = objYesterday.value("fx").toString();// 风向
    mDay[0].aqi = objYesterday.value("aqi").toDouble();// 天气污染指数

    //3.解析forecast中5天的数据
    QJsonArray forecastArr = objData.value("forecast").toArray();
    for (int i=0;i<5;i++){
        QJsonObject objForecast = forecastArr[i].toObject();

        mDay[i+1].date=objForecast.value("ymd").toString();
        mDay[i+1].week=objForecast.value("week").toString();
        mDay[i+1].type=objForecast.value("type").toString();

        QString s;// "high":"高温  18°","low":"低温  6°"
        s=objForecast.value("high").toString().split(" ").at(1);// 18°
        s=s.left(s.length()-1);// 18
        mDay[i+1].high=s.toInt();

        s=objForecast.value("low").toString().split(" ").at(1);// 6°
        s=s.left(s.length()-1);// 6
        mDay[i+1].low=s.toInt();

        mDay[i+1].fl = objForecast.value("fl").toString();// 风力
        mDay[i+1].fx = objForecast.value("fx").toString();// 风向

        mDay[i+1].aqi = objForecast.value("aqi").toDouble();// 天气污染指数
    }

    //4.解析今天的数据
    mToday.ganmao = objData.value("ganmao").toString();
    mToday.wendu = objData.value("wendu").toString().toInt();
    mToday.shidu = objData.value("shidu").toString();
    mToday.pm25 = objData.value("pm25").toInt();
    mToday.quality = objData.value("quality").toString();

    //5. 在forecast中的第一个数组元素，也就是今天的数据
    mToday.type = mDay[1].type;
    mToday.fx = mDay[1].fx;
    mToday.fl = mDay[1].fl;
    mToday.high = mDay[1].high;
    mToday.low = mDay[1].low;

    //6. 更新UI
    updateUI();

    //7. 绘制曲线
    ui->lblHighCurve_4->update();//触发绘画事件
    ui->lblLowCurve_4->update();
}

// 更新UI
void Weather::updateUI()
{
    // 日期
    ui->lbDate_4->setText(QDateTime::fromString(mToday.date,"yyyyMMdd").toString("yyyy/MM/dd")+" "+mDay[1].week);
    ui->lbCity_4->setText(mToday.city);// 城市

    //1. 更新今天
    ui->lbTemp_4->setText(QString::number(mToday.wendu)+"°");
    ui->lbType_4->setText(mToday.type);
    ui->lbTypeIcon_4->setPixmap(mTypeMap[mToday.type]);// 天气类型图标
    ui->lbLowHigh_4->setText(QString::number(mToday.low)+"~"+QString::number(mToday.high)+"°C");
    ui->lbGaoMao_4->setText("感冒指数："+mToday.ganmao);
    ui->lbWindFx_4->setText(mToday.fx);
    ui->lbWindFl_4->setText(mToday.fl);
    ui->lbPm25_4->setText(QString::number(mToday.pm25));
    ui->lbShiDu_4->setText(mToday.shidu);
    ui->lblQuality_4->setText(mToday.quality);// 空气质量

    //2. 更新6天
    for (int i=0;i<6;i++) {
        // 日期
        mWeekList[i]->setText("周"+mDay[i].week.right(1));// mDay[i].week="星期x"
        ui->lblWeek0_4->setText("昨天");
        ui->lblWeek1_4->setText("今天");
        ui->lblWeek2_4->setText("明天");
        // 时间
        QStringList ymdList=mDay[i].date.split("-");// mDay[i].date="2023-03-26"
        mDateList[i]->setText(ymdList[1]+"/"+ymdList[2]);
        // 天气类型
        mTypeList[i]->setText(mDay[i].type);
        // 天气类型图标
        mTypeIconList[i]->setPixmap(mTypeMap[mDay[i].type]);
        // 空气质量
        if(mDay[i].aqi >= 0 && mDay[i].aqi <= 50){
            mAqiList[i]->setText("优");
            mAqiList[i]->setStyleSheet("background-color:rgb(121,184,0);");
        }else if(mDay[i].aqi > 50 && mDay[i].aqi <= 100){
            mAqiList[i]->setText("良");
            mAqiList[i]->setStyleSheet("background-color:rgb(255,187,23);");
        }else if(mDay[i].aqi > 100 && mDay[i].aqi <= 150){
            mAqiList[i]->setText("轻度");
            mAqiList[i]->setStyleSheet("background-color:rgb(255,87,97);");
        }else if(mDay[i].aqi > 150 && mDay[i].aqi <= 200){
            mAqiList[i]->setText("中度");
            mAqiList[i]->setStyleSheet("background-color:rgb(235,17,27);");
        }else if(mDay[i].aqi > 200 && mDay[i].aqi <= 250){
            mAqiList[i]->setText("重度");
            mAqiList[i]->setStyleSheet("background-color:rgb(170,0,0);");
        }else {
            mAqiList[i]->setText("严重");
            mAqiList[i]->setStyleSheet("background-color:rgb(110,0,0);");
        }
        // 风向
        mFxList[i]->setText(mDay[i].fx);
        mFlList[i]->setText(mDay[i].fl);//风力

    }

}

// 重写父类的eventFilter方法
bool Weather::eventFilter(QObject *watched, QEvent *e)
{
    if(watched == ui->lblHighCurve_4 && e->type() == QEvent::Paint){
        paintHighCurve();
    }
    if(watched == ui->lblLowCurve_4 && e->type() == QEvent::Paint){
        paintLowCurve();
    }

    // 事件过滤中，只对特殊的进行处理，其他无关的，走原路[父类的eventFilter]
    return QWidget::eventFilter(watched,e);
}

//绘制高温曲线
void Weather::paintHighCurve()
{
    QPainter painter(ui->lblHighCurve_4);// 画家类
    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);
    // 1. 获取x轴坐标
    int pointX[6]={0};
    for(int i=0;i<6;i++){
        pointX[i]=mWeekList[i]->pos().x()+(mWeekList[i]->width()/4);
    }
    // 2. 获取y轴坐标
    int tempSum=0;//总和
    int tempAverage=0;//平均
    for(int i=0;i<6;i++){
        tempSum+=mDay[i].high;
    }
    tempAverage=tempSum/6; // 最高温的平均值
    // 计算y轴坐标
    int pointY[6]={0};
    int yCenter = ui->lblHighCurve_4->height()/2;
    for(int i=0;i<6;i++){
        pointY[i]=yCenter - ((mDay[i].high-tempAverage)*INCREMENT);
    }

    // 3. 开始绘制
    // 3.1 初始化画笔相关
    QPen pen=painter.pen();
    pen.setWidth(1);    //设置画笔的宽度
    pen.setColor(QColor(255,170,0)); //设置画笔的颜色
    painter.setPen(pen);//给画家设置画笔
    painter.setBrush(QColor(255,170,0));//设置画刷的颜色-内部填充的颜色
    // 3.2 画点、画文字
    for(int i=0;i<6;i++){
        // 显示点
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),POINT_RADIUS,POINT_RADIUS);
        // 显示温度
        painter.drawText(pointX[i]-TEXT_OFFSET_X,pointY[i]-TEXT_OFFSET_X,
                         QString::number(mDay[i].high));
    }
    // 3.3绘制曲线
    for(int i=0;i<6-1;i++){
        if(i == 0){
            pen.setStyle(Qt::DotLine);//虚线
            painter.setPen(pen);
        }else{
            pen.setStyle(Qt::SolidLine);//虚线
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
    }
}

//绘制低温曲线
void Weather::paintLowCurve()
{
    QPainter painter(ui->lblLowCurve_4);// 画家类
    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing,true);
    // 1. 获取x轴坐标
    int pointX[6]={0};
    for(int i=0;i<6;i++){
        pointX[i]=mWeekList[i]->pos().x()+(mWeekList[i]->width()/4);
    }
    // 2. 获取y轴坐标
    int tempSum=0;//总和
    int tempAverage=0;//平均
    for(int i=0;i<6;i++){
        tempSum+=mDay[i].low;
    }
    tempAverage=tempSum/6; // 最高温的平均值
    // 计算y轴坐标
    int pointY[6]={0};
    int yCenter = ui->lblLowCurve_4->height()/2;
    for(int i=0;i<6;i++){
        pointY[i]=yCenter - ((mDay[i].low-tempAverage)*INCREMENT);
    }

    // 3. 开始绘制
    // 3.1 初始化画笔相关
    QPen pen=painter.pen();
    pen.setWidth(1);    //设置画笔的宽度
    pen.setColor(QColor(0,255,255)); //设置画笔的颜色
    painter.setPen(pen);//给画家设置画笔
    painter.setBrush(QColor(0,255,255));//设置画刷的颜色-内部填充的颜色
    // 3.2 画点、画文字
    for(int i=0;i<6;i++){
        // 显示点
        painter.drawEllipse(QPoint(pointX[i],pointY[i]),POINT_RADIUS,POINT_RADIUS);
        // 显示温度
        painter.drawText(pointX[i]-TEXT_OFFSET_X,pointY[i]-TEXT_OFFSET_X,
                         QString::number(mDay[i].low));
    }
    // 3.3绘制曲线
    for(int i=0;i<6-1;i++){
        if(i == 0){
            pen.setStyle(Qt::DotLine);//虚线
            painter.setPen(pen);
        }else{
            pen.setStyle(Qt::SolidLine);//虚线
            painter.setPen(pen);
        }
        painter.drawLine(pointX[i],pointY[i],pointX[i+1],pointY[i+1]);
    }
}

// 处理HTTP服务返回数据
void Weather::onReplied(QNetworkReply *reply)
{
    // 响应的状态码为200，表示请求成功
    int statusCode=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<"statusCode:"<< statusCode;

    // 请求方法
    qDebug()<<"operation:"<< reply->operation();
    qDebug()<<"url:"<< reply->url();
    // 响应头
    qDebug()<<"raw Header:"<< reply->rawHeaderList();

    if(statusCode!=200 || reply->error()!=QNetworkReply::NoError)
    {
        qDebug()<<"error:"<< reply->errorString();
        QMessageBox::warning(this,"天气","请求数据失败",QMessageBox::Ok);
    }
    else
    {
        QByteArray byteArray=reply->readAll();
        //qDebug()<<"readAll:"<< byteArray.data();

        parseJson(byteArray);// 分析Json数据
    }

    reply->deleteLater();//延时释放,防止堆区的接收数据泄漏
}

// 搜索
void Weather::on_btnSearch_4_clicked()
{
    QString cityName=comboBox->currentText();
    getWeatherInfo(cityName);
}

void Weather::on_pushButton_clicked()
{
    this->close();
    mainwindow *pic=new mainwindow();
    pic->show();
}
