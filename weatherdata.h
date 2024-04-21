#ifndef WEATHERDATA_H
#define WEATHERDATA_H
#pragma execution_character_set("utf-8")
#include <QString>
class Today{
public:
    QString date;   // 日期
    QString city;   //城市

    QString ganmao; //感冒

    int wendu;  // 温度
    QString shidu;  // 湿度
    int pm25;   //   pm2.5
    QString quality;    //质量

    QString type;   // 天气类型

    QString fx; //风向
    QString fl; //风力

    int high;   //高温
    int low;    //低温

    // 构造函数，默认值
    Today(){
        date="2023-03-26";   // 日期
        city="北京";   //城市

        ganmao="感冒指数"; //感冒

        wendu=0;  // 温度
        shidu="0%";  // 湿度
        pm25=0;   //   pm2.5
        quality="无数据";    //质量

        type="多云";   // 天气类型

        fx="南风"; //风向
        fl="2级"; //风力

        high=30;   //高温
        low=18;    //低温
    }
};

class Day{
public:
    QString date;   // 日期
    QString week;   // 星期x

    QString type;   // 天气类型

    int high;   //高温
    int low;    //低温

    QString fx; //风向
    QString fl; //风力

    int aqi;    // 天气污染指数

    // 构造函数，默认值
    Day(){
        date="2023-03-26";   // 日期
        week="周日";   //星期x

        type="多云";   // 天气类型

        high=0;   //高温
        low=0;    //低温

        fx="南风"; //风向
        fl="2级"; //风力

        aqi=0;  // 天气污染指数
    }
};


#endif // WEATHERDATA_H
