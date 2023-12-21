#ifndef CAPITALISMKASYNO_H
#define CAPITALISMKASYNO_H
#include "capitalismstorage.h"
#include <QObject>
#include "capitalismgame.h"
#include <vector>
#include <utility>
#include <QThread>
#include <QTimer>
#include <QMutex>
class CapitalismGame;
class CapitalismStorage;

class CapitalismKasyno: public QObject
{
    Q_OBJECT
public:
     CapitalismKasyno(QThread *kasyno_, QString nick_, std::string room_, QString data_, double betMoney_, CapitalismStorage* capitalismStorage_, CapitalismGame* capitalismGame_);
    ~CapitalismKasyno();
    void replaceSameEnteries(std::vector<std::pair<std::string, double>>& playersAndTheirBets_v);
    std::vector<std::pair<std::string, double> > gameLogic(std::vector<std::pair<std::string, double>>& playersAndTheirBets_v);
    // void setBetMoney(float betMoney_ = -1);
public slots:
    void run(double betMoney_, QString nick_);
    void passer();
    void gameFinished();
    void debugIt(std::string str);

signals:
    void sendPost(const QByteArray);
private:
    CapitalismStorage* capitalismStorage;
    CapitalismGame* capitalismGame;
    std::string nick;
    std::string room;
    QMutex* mutex;
    QString data;
    std::vector<std::pair<std::string, double>> playersAndTheirBets_v;
    QThread *kasyno;
    double betMoney;
    QTimer* timer;
    double prize;
};

#endif // CAPITALISMKASYNO_H
