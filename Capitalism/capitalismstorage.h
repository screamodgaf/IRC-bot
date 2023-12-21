#ifndef CAPITALISMSTORAGE_H
#define CAPITALISMSTORAGE_H
#include <vector>
//#include "capital.h"
#include <QThread>
#include <QObject>
#include "Capitalism/stockreadandsave.h"
#include "Capitalism/capitalismgame.h"
#include <QDir>
#include <fstream>

class LoadLogs;
class Capital;
class CapitalismGame;

class CapitalismStorage: public QObject
{
    Q_OBJECT
public:
    CapitalismStorage(QObject *parent = nullptr, LoadLogs *loadLogs_ = nullptr, CapitalismGame *capitalismGame_ = nullptr);
    void attachAssetsToUser();
    std::vector<std::string> get_infoAllCapitalists();
    bool determineUser(std::string nick);
    void addMoney(std::string nick, double money);
    void addRank(std::string nick, int rank);
    void addStock(std::string nick, double amountBought, std::string stockName);
    std::vector<Capital>& get_capitalists_v();
    void createEntry(std::string nick, double money); // in capitalists_v
    Capital *getUserFromNick(std::string nick);
    void saveCapitalistsToFile();
    void determineLevels();
    void determineLevel();
    CapitalismGame* getCapitalismGamePtr();
public slots:

private:
    std::vector<Capital> capitalists_v; //most important container
    std::vector<std::string>* listOfCapitalists_v; //this is from file
    std::vector< std::vector<std::string> > users_andSingularAssets_v;
    LoadLogs *loadLogs;
    Capital *capital;
    Capital *capitalTemp; // for carrying return of element of listOfCapitalists_v;
    std::string temp;
    std::vector<int> levels_v;
    CapitalismGame *capitalismGame;
    QDir directory;
    std::ofstream file;
private:

};

#endif // CAPITALISMSTORAGE_H
