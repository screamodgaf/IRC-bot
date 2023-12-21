#ifndef CAPITALISMGAME_H
#define CAPITALISMGAME_H
#include <QObject>
#include <QSslSocket>
#include <vector>
#include "capital.h"
#include "loadlogs.h"
#include "capitalismstorage.h"
#include "extract_nickandroom.h"
#include <termcolor.hpp>
#include <antiflood.h>
#include "capitalismkasyno.h"
#include <QMutex>
class CapitalismStorage;
class LoadLogs;
class Capital;
class AntiFlood;
class CapitalismKasyno;

class CapitalismGame: public QObject
{
    Q_OBJECT
public:
    explicit CapitalismGame(QObject *parent = nullptr, AntiFlood *antiFlood_ = nullptr, QMutex* mutex_ = nullptr);
    ~CapitalismGame();
    void run(QString data_, QSslSocket *mSocket_, CapitalismStorage* capitalismStorage_);
    bool getNickAndRoom();
    void checkKeywords();
    bool atomizeCommand(QString data_ , std::string command);
    void information();
    std::string informationOfUser(std::string nick);
    bool checkIfNumber(std::string &s);
    std::string round_float(double money);
    bool get_kasyno_is();

public slots:
    void doPosting(const std::string &newsText);
    void doPosting2(QByteArray post);
    void fineCapitalistFlooder(std::vector<std::string> flooder_v);
    void threadFinished();


signals:
    void betNickSignal(double betMoney_, QString nick_);

private:
    QString data;
    QSslSocket *mSocket;
    std::string nick;
    std::string myNick;
    std::string room;
    std::string line;
    QMutex * mutex;
    CapitalismStorage* capitalismStorage;
    std::vector<std::string> commandElements_v;
    Extract_NickAndRoom extract_NickAndRoom;
    static int earningCounter;
    AntiFlood* antiFlood;
    bool kasyno_is;


    QThread* kasyno;
    CapitalismKasyno* capitalismKasyno;
private:
    void earningMoney();
    void startKasyno(double betMoney_ = -12.0,  QString nick = "wefdcdww");

};

#endif // CAPITALISMGAME_H
