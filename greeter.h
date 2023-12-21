#ifndef GREETER_H
#define GREETER_H

#include <QObject>
#include <QTcpSocket>
#include "loadlogs.h"

#include <QTimer>

class Greeter : public QObject
{
    Q_OBJECT
public:
    explicit Greeter(QObject *parent = nullptr);



    void run(const QString data_, QTcpSocket *mSocket, int& indexLines_, std::string myNick_, LoadLogs* loadlogs);
    void pickGreetingLine();
    void checkJoinPresence();
    static bool greetingsSent_is;
    static unsigned int Indexgreetings_v;


private:
    QString data;
    LoadLogs *loadLogs;
    QTcpSocket *mSocket;
    std::string nick;
    std::string myNick;
    std::string room;
    std::string line;
    int* indexLines;

signals:
void postSignal(QByteArray);
private:
    std::vector<std::string>* greetings_v;
public slots:
    void doPosting();

};

#endif // GREETER_H
