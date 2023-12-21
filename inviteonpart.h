#ifndef INVITEONPART_H
#define INVITEONPART_H
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class LoadLogs;

class InviteOnPart: public QObject
{
    Q_OBJECT
public:
    explicit InviteOnPart(QObject *parent = nullptr);
    void run(const QString data_, QTcpSocket *mSocket, std::string myNick_, LoadLogs* loadlogs);
    void pickGreetingLine();
    void checkPartPresence();
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

private:
    std::vector<std::string>* greetings_v;
public slots:
    void postInvite();
};


#endif // INVITEONPART_H
