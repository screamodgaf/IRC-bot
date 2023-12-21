#ifndef NEWS1_H
#define NEWS1_H
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include "parser.h"
#include <QTcpSocket>
#include <QThread>
class News1  : public QObject
{
    Q_OBJECT
public:
    explicit News1(QObject *parent = nullptr);
    void makeRequest();
    void checkKeyword(QString data_);
public slots:
    void readyRead(QNetworkReply *replay);
signals:
    void mySignal(const QByteArray);
private:
    void postToIRC(std::string newsText);
    std::string clear(std::string& text);

    QNetworkAccessManager *manager;
    QNetworkRequest  request;
    std::string website;

    QString data;

    std::string room;
};

#endif // News1_H
