#include "weatherapiforecast.h"
#include <QThread>
#include <QByteArray>
#include "extract_nickandroom.h"
#include "parser.h"
#include <iostream>
#include <vector>

WeatherApiForecast::WeatherApiForecast(QObject *parent) : QObject(parent)
{
    qDebug() << "WeatherApiForecast Ctor";
    manager = new QNetworkAccessManager();

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(readyRead(QNetworkReply*)));
}

std::string WeatherApiForecast::clearString(std::string str)
{
  std::cout << "WeatherApiForecast::clearString\n";
    std::string temp;
    for (char c : str) {
         if(std::isdigit(c) || c =='-')
            temp.push_back(c);
    }
    return temp;
}

void WeatherApiForecast::makeRequest()
{
 std::cout << "WeatherApiForecast::makeRequest()\n";

    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration().);


    QString url = "http://api.openweathermap.org/data/2.5/forecast?appid=6ad1576301dad58f45d173da350f3d75&q=" + indexCore ;
 qDebug() << "url: " << url;



   std::cout << "indexCore.toStdString(): "<< indexCore.toStdString() << std::endl;
   // QString url = "https://en.tutiempo.net/" + indexCore + ".html";

    request.setUrl(QUrl(url));
    QString text = "User-Agent" ;
    QString text2 =  "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) Qt/4.7.4" ;
    request.setRawHeader(QByteArray(text.toLocal8Bit() ) ,
                         QByteArray(text2.toLocal8Bit()));

    manager->get(request);

}

QString WeatherApiForecast::getDayOfWeek(QString data, int index){

std::cout << "WeatherApiForecast::getDayOfWeek()\n";
    QString hour = data.mid(data.indexOf(" "), data.size());
    data = data.mid(0, data.indexOf(" "));
    //        "2023-01-08 06:00:00"
    QString year, month, day;
    year = data.mid(0, data.indexOf("-"));
    month = data.mid(5, 2);
    day = data.mid(8, 2);
    QString dataTemp = data;

    QString lineToPost;
    if(hour.indexOf("03:00:00")!=-1){
        hour.replace("03:00:00", "3am");
        lineToPost = "\u000311,12 "  + hour  ;
    }
    if(hour.indexOf("12:00:00")!=-1){
        hour.replace("12:00:00", "12pm");
        lineToPost = "\u000307,8 " + hour ;
    }

qDebug() << "data: " << dataTemp << "\n";
    QDate dayDate;
    dayDate.setDate(year.toInt(), month.toInt(), day.toInt());
    qDebug() << "dayDate: " << dayDate << "\n";
    qDebug() << "dayDate.dayOfWeek(): " << dayDate.dayOfWeek() << "\n";
    QString dayLiteral;
    switch (dayDate.dayOfWeek()) {
    case 1:
        dayLiteral = "\u000304Pon " + lineToPost;
        break;
    case 2:
         dayLiteral = "\u000313Wt " + lineToPost;
         break;
    case 3:
         dayLiteral = "\u000307Śr " + lineToPost;
         break;
    case 4:
         dayLiteral = "\u000306Cz " + lineToPost;
         break;
    case 5:
         dayLiteral = "\u000307Pią " + lineToPost;
         break;
    case 6:
         dayLiteral = "\u000308Sob " + lineToPost;
         break;
    case 7:
         dayLiteral = "\u000309Niedz " + lineToPost;
        break;

    }

    qDebug() << "dayLiteral: " << dayLiteral << "\n";
    if(index == 0)
        dayLiteral += "\x02\x03Teraz ";
    return dayLiteral;
}

void WeatherApiForecast::readyRead(QNetworkReply *replay)
{
    std::cout << "WeatherApiForecast::readyRead()\n";
    //qDebug() << "WOOHOO: " << replay->readAll();

    QByteArray dataTemp = replay->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(dataTemp);
    QJsonObject qJsonObject = doc.object();

    QString cityInfo = getCityInfo(qJsonObject);

    //looping through all whats inside "list" in our main object - qJsonObject {}:
    //qJsonObject has 5 elements, one of them is list[] array, so im creating QJsonArray from QJsonObject:
    QJsonArray listArrayOfObjectsInList = qJsonObject["list"].toArray(); //0,1,2,3..." all whats inside "list" in qJsonObject
    //this QJsonArray has 40 Objects in it, so im getting into one of them by listArrayOfObjectsInList[0]:
    //continuing in getCloudInfo()

    doPosting(cityInfo.toStdString(), mSocketInner);
    QThread::msleep(250);
    mSocketInner->flush();


    QString lineToPost;
    for (int i = 0; i < listArrayOfObjectsInList.size(); ++i) {
        //qDebug() << listArrayOfObjects[i];
        QString data = getDate(listArrayOfObjectsInList, i); //i
        if(i!= 0 && (data.indexOf("03:00:00")==-1 && data.indexOf("12:00:00")==-1))
            continue;

        QString temp = getTempInfo(listArrayOfObjectsInList, i);
        QString clouds = getCloudInfo(listArrayOfObjectsInList, i);

        QString day = getDayOfWeek(data, i);

        lineToPost = day + " " + temp+ " " + clouds;


        doPosting(lineToPost.toStdString(), mSocketInner);
        QThread::msleep(250);
        mSocketInner->flush();


    }





}

//=========
QString WeatherApiForecast::getCityInfo(QJsonObject& qJsonObject){
    QJsonObject objCity = qJsonObject["city"].toObject(); //within qJsonObject

    //city info:
    int cityPopulation = objCity["population"].toInt();
    QString cityInfo = "\u000306,10Miasto: " + objCity["name"].toString() + ", Kraj: " + objCity["country"].toString() +
            ", Populacja: " + QString::number(cityPopulation) ;
//    qDebug() << cityInfo;
    return cityInfo;
}

QString WeatherApiForecast::getDate(QJsonArray& listArrayOfObjectsInList, int index){
    QJsonObject qJsonObjectData =  listArrayOfObjectsInList[index].toObject();
//    qDebug() << qJsonObjectData["dt_txt"].toString();
    return qJsonObjectData["dt_txt"].toString();

}

QString WeatherApiForecast::getTempInfo(QJsonArray& listArrayOfObjects, int index){
    QJsonObject objectsInList = listArrayOfObjects[index].toObject();
    QJsonObject objectsInMain = objectsInList["main"].toObject();
    QString lineToPost;
    double temp = objectsInMain["temp"].toDouble();
    temp = temp - 273; //from Kelvin to Celsius,
    QString tempSrt = "\u000308,12Temp: " + QString::number(temp) + " *C ";

    double feels_likeTemp = objectsInMain["feels_like"].toDouble();
    feels_likeTemp = feels_likeTemp  - 273;

    tempSrt += "\u000306,3 (" + QString::number(feels_likeTemp) + " *C) ";

//    qDebug() << tempSrt;
    return tempSrt;
}

QString WeatherApiForecast::getCloudInfo(QJsonArray& listArrayOfObjects, int index){
        //now im inside listArrayOfObjectsInList[0], which is an object, so im creating object:
        QJsonObject objectsInList = listArrayOfObjects[index].toObject();
        //using "weather" key on the object, im getting into weather Array, so im creating array:
        QJsonArray objectsInMain = objectsInList["weather"].toArray();
        //objectsInMain[0] is object, so im creating object:
        QJsonObject objectMaininWeather = objectsInMain[0].toObject();
        //accesing clouds info by using key objectMaininWeather["description"] on the object:
        QString conditions =  "\u000303,7Niebo: " + objectMaininWeather["description"].toString();
//        qDebug() << "Warunki: " << objectMaininWeather["description"].toString();
        return conditions;
}



    //=============

void WeatherApiForecast::WeatherForecastKeyWord(QString data, QTcpSocket *mSocket)
{
    mSocketInner = mSocket;
    std::cout << "WeatherApiForecast::WeatherForecastKeyWord" << std::endl;

    if(data.indexOf("!forece ")!=-1)
    {
        Extract_NickAndRoom extract;
        room = extract.extractRoom(data);


    int indexBegin = data.indexOf("!forece "); //6
    int indexEnd = data.indexOf("\r\n");

    indexCore = data.mid(indexBegin+8,(indexEnd -(indexBegin+8) ));

    makeRequest();

    }
}



void WeatherApiForecast::doPosting(std::string string, QTcpSocket *mSocket)
{


     std::string lineToPost;
     lineToPost = "PRIVMSG " + room + " :" + string + "\r\n ";

     const char *send = lineToPost.c_str();
     mSocket->write(send);
     mSocket->flush();
 }
