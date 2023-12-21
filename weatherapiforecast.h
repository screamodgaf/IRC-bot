#ifndef WEATHERAPIFORECAST_H
#define WEATHERAPIFORECAST_H


#include <QDebug>
#include <QObject>
#include <QProcess>
#include <QTcpSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>


class WeatherApiForecast: public QObject
{
    Q_OBJECT
public:
    explicit WeatherApiForecast(QObject *parent = nullptr);
    void WeatherForecastKeyWord(QString data, QTcpSocket *mSocket);

    void doPosting(std::string string, QTcpSocket *mSocket);

    //for network:
    void makeRequest();
    void checkKeyword(QString data_);
private:
    std::string room;
    void postToIRC(std::string newsText);
    std::string clear(std::string& text);
    std::string clearString(std::string str);
    QNetworkAccessManager *manager;
    QNetworkRequest  request;
    std::string website;
    QString indexCore; // city
    QString data;

    std::string postDataStr;
     QTcpSocket *mSocketInner;

     QString getCityInfo(QJsonObject& qJsonObject);
     QString getDate(QJsonArray& listArrayOfObjectsInList, int index);
     QString getTempInfo(QJsonArray& listArrayOfObjects, int index);
     QString getCloudInfo(QJsonArray& listArrayOfObjects, int index);
     QString getDayOfWeek(QString data, int index);

signals:
    void mySignal(const QByteArray);
public slots:
    void readyRead(QNetworkReply *replay);

};


#endif // WEATHERAPIFORECAST_H
