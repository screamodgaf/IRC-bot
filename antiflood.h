#ifndef ANTIFLOOD_H
#define ANTIFLOOD_H
#include <QObject>
#include "extract_nickandroom.h"
#include <vector>
#include <QTcpSocket>
#include <QTimer>
#include <chrono>


class IgnoredNicks;

typedef std::chrono::time_point<std::chrono::high_resolution_clock> czas;

class AntiFlood : public QObject
{
    Q_OBJECT
public:
    explicit AntiFlood(QObject *parent = nullptr);
    void receiveLine(QString& newLine, QTcpSocket* socket, IgnoredNicks* ignoredNicks_);

signals:
    void floodingUser(std::vector<std::string> flooder_v);
    void unbanUser();
    void unbanInfoPost(QByteArray lineToPost);
public :
    void unBan();

private:
    void sendMessages(std::string nick, std::string room);


private:

    Extract_NickAndRoom extract;
    std::vector<std::pair<std::vector<std::string>, czas>> linesFromChat_v;
    std::vector<std::string> sameLines_v; //aniflood


    std::string nick;
    std::string room;
    std::string line;
    std::string lineToPost;


    //void(AntiFlood::*lala)(); //pointer to a member function

//    bool is_check;
    QTcpSocket* m_socket;
    IgnoredNicks* ignoredNicks;

private:

    void sendIT(std::string line );

};

#endif // ANTIFLOOD_H
