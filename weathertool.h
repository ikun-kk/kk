#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H
#pragma execution_character_set("utf-8")
#include <QMap>
#include <QString>
#include <QFile>
#include <QIODevice>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonParseError>

class WeatherTool{
private:
    /*
     * 也可以使用inline内联声明变量，就能解决在头文件中定义了静态变量，然后在多个源文件中包含了这个头文件，
     * 就会出现同一个变量被多次定义的情况，从而导致链接错误,所以放在.cpp中
     * 注意：使用inline需要在.pro修改CONFIG += c++17
     *
     * 如果使用inline就不能在类外进行 初始化；请注释QMap<QString,QString> WeatherTool::mCityMap={};
    */
    //inline static QMap<QString,QString> mCityMap={};// Key=城市，value=编码

    static QMap<QString,QString> mCityMap;// Key=城市，value=编码
    static void initCityMap(){
        // 1.读取文件
        QString filePath=":/citycode.json";
        QFile file(filePath);
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QByteArray json=file.readAll();
        file.close();

        // 2. 解析，并写入到Map
        QJsonParseError err;
        QJsonDocument doc=QJsonDocument::fromJson(json,&err);
        if(err.error!=QJsonParseError::NoError){
            return;
        }
        if(!doc.isArray()){
            return;
        }

        QJsonArray cities = doc.array();
        for(int i=0;i<cities.size();i++){
            QString city = cities[i].toObject().value("city_name").toString();
            QString code = cities[i].toObject().value("city_code").toString();
            if(code.size()>0){
                mCityMap.insert(city,code);
            }
        }
    }

public:
    static QString getCityCode(QString cityName)
    {
        if(mCityMap.isEmpty()){
            initCityMap();
        }
        QMap<QString,QString>::iterator it=mCityMap.find(cityName);
        // 北京 / 北京市
        if(it == mCityMap.end()){
            it=mCityMap.find(cityName+"市");
        }

        if(it != mCityMap.end()){
            return  it.value();
        }
        return "";
    }
};

/*
 * 也可以使用inline内联声明变量，就能解决在头文件中定义了静态变量，然后在多个源文件中包含了这个头文件，
 * 就会出现同一个变量被多次定义的情况，从而导致链接错误,所以放在.cpp中
 * 注意：使用inline需要在.pro修改CONFIG += c++17
 *
 * 如果使用inline就不能在类外进行 初始化；请注释QMap<QString,QString> WeatherTool::mCityMap={};
*/

// 静态成员变量 需要 在类外进行 初始化
QMap<QString,QString> WeatherTool::mCityMap={};



#endif // WEATHERTOOL_H
