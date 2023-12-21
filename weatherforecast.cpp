#include "weatherforecast.h"
#include <QThread>
#include <QByteArray>
#include "extract_nickandroom.h"
#include "parser.h"
#include <iostream>
#include <vector>

WeatherForecast::WeatherForecast(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager();

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(readyRead(QNetworkReply*)));
}

std::string WeatherForecast::clearString(std::string str)
{
    std::cout << "WeatherForecast::clearString\n";
    std::string temp;
    for (char c : str) {
         if(std::isdigit(c) || c =='-')
            temp.push_back(c);
    }
    return temp;
}

void WeatherForecast::makeRequest()
{
std::cout << "WeatherForecast::makeRequest()" << std::endl;

    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration().);


    QString url = "https://en.tutiempo.net/" + indexCore + ".html";




   std::cout << "command: "<< indexCore.toStdString() << std::endl;
   // QString url = "https://en.tutiempo.net/" + indexCore + ".html";

    request.setUrl(QUrl(url));
    QString text = "User-Agent" ;
    QString text2 =  "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) Qt/4.7.4" ;
    request.setRawHeader(QByteArray(text.toLocal8Bit() ) ,
                         QByteArray(text2.toLocal8Bit()));

    manager->get(request);

}

void WeatherForecast::readyRead(QNetworkReply *replay)
{
    std::cout << "WeatherForecast::readyRead(QNetworkReply *replay)\n";
    //qDebug() << "WOOHOO: " << replay->readAll();

    QByteArray dataTemp = replay->readAll();
    website = dataTemp.toStdString();

    Parser p;

    std::string postDataStr;
    for (int i = 0; i < 7; ++i) {

        postDataStr = p.goIntoTag(website, "span class=\"day\"", i) + "\35 " +
        "\u000303,7dzien: " + clearString( p.goIntoTag(website, "span class=\"t max\"", i)) + "*C " +
        "\u000308,12 noc: " + clearString( p.goIntoTag(website, "span class=\"t min\"", i)) + "*C " +
        "\u000306,10 wiatr: " + clearString( p.goIntoTag(website, "span class=\"vv v\"", i)) + "km/h \n";

        //check IF doesnt post code if city not found:
        if(postDataStr.find("http-equiv=\"content-type\"")!= std::string::npos)
         {
            std::cout << "city not found" << std::endl;
            return;
         }

        doPosting(postDataStr, mSocketInner);
        QThread::msleep(250);
        mSocketInner->flush();

    }



}



void WeatherForecast::WeatherForecastKeyWord(QString data, QTcpSocket *mSocket)
{
    mSocketInner = mSocket;
    std::cout << "lalalalala@@@@@@@@@@@@@lalala" << std::endl;

    if(data.indexOf("!foreca ")!=-1)
    {
        Extract_NickAndRoom extract;
        room = extract.extractRoom(data);


    int indexBegin = data.indexOf("!foreca "); //6
    int indexEnd = data.indexOf("\r\n");

    indexCore = data.mid(indexBegin+8,(indexEnd -(indexBegin+8) ));

    makeRequest();

    }
}



void WeatherForecast::doPosting(std::string string, QTcpSocket *mSocket)
{
    std::cout << "WeatherForecast::doPosting\n";

     std::string lineToPost;
     lineToPost = "PRIVMSG " + room + " :" + string + "\r\n ";

     const char *send = lineToPost.c_str();
     mSocket->write(send);
     mSocket->flush();
 }
