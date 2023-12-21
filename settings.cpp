#include "settings.h"

Settings::Settings(QObject *parent)
    : QObject{parent}
{}

QString Settings::getMyNick()
{
    return nick;
}

QString Settings::getMyRoom()
{
    return room;
}

QString Settings::getMyPort()
{
    return port;
}

QString Settings::getMyServer()
{
    return server;
}

QString Settings::getProxyUSer()
{
    return proxyUSer;
}

QString Settings::getProxyPassword()
{
    return proxyPassword;
}

QString Settings::getProxyIP()
{
    return proxyIP;
}

QString Settings::getProxyPort()
{
    return proxyPort;
}

QString Settings::nick = "Hikari";
QString Settings::room = "#test111";

QString Settings::port = "6667";
QString Settings::server = "irc.rizon.net"; //irc server address

QString Settings::proxyUSer = ""; //your proxy username
QString Settings::proxyPassword = ""; //your proxy password

QString Settings::proxyPort = ""; // your proxy port
QString Settings::proxyIP = ""; //your proxy ip


