#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
private:
    static QString nick;
    static QString room;
    static QString port;
    static QString server;
    static QString proxyUSer;
    static QString proxyPassword;
    static QString proxyPort;
    static QString proxyIP;

public:
    explicit Settings(QObject *parent = nullptr);

    static QString getMyNick();
    static QString getMyRoom();
    static QString getMyPort();
    static QString getMyServer();
    static QString getProxyUSer();
    static QString getProxyPassword();
    static QString getProxyIP();
    static QString getProxyPort();


signals:
};

#endif // SETTINGS_H


