#ifndef WEATHERFORECAST_H
#define WEATHERFORECAST_H
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
class WeatherForecast: public QObject
{
    Q_OBJECT
public:
    explicit WeatherForecast(QObject *parent = nullptr);
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
signals:
    void mySignal(const QByteArray);
public slots:
    void readyRead(QNetworkReply *replay);

};

#endif // WEATHERFORECAST_H
