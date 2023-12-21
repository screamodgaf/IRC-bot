#ifndef CAPITAL_H
#define CAPITAL_H
#include <iostream>
#include <vector>
#include "capitalismgame.h"
#include "capitalismstorage.h"
#include <termcolor.hpp>
class CapitalismGame;
class CapitalismStorage;

class Capital
{
public:
    explicit Capital(CapitalismStorage *capitalismStorage_ = nullptr);
    void setNick(std::string nick_);
    void setMoney(double money_);
    void setFactory(std::string factory_);
    void setRank(int rank_);
    //stocks:
    void setAllStocks(std::vector<std::vector<std::string> > &singleUserStocks_v_); //initially
    void addNewStock(std::string newStockName, int amountBought, std::string boughtPrice);
    std::string sellStock(std::string sellStockName, double amountSold, double currentPrice);
    std::string checkStock(std::string stockName, double currentPrice);
    std::string& getNick();
    double &getMoney();
    int& getRank();
    std::vector<std::vector<std::string>> &getStocks();
    std::string& getFactory();
    std::vector<std::string>& getAttributesForSave();
    CapitalismStorage* capitalismStorage;
    CapitalismGame* capitalismGame;
signals:
    void signalGain(std::string gain);
private:
    std::string nick;
    double money;
    std::string factory;
    int rank;
    std::vector<std::vector<std::string>> singleUserStocks_v;
    std::string postAboutGainFromEachEntry = "";
    double gainTotal;
};

#endif // CAPITAL_H
