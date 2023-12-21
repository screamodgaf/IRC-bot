/*************** DOCUMENTATION *************************************************
 *
 *  std::string line = "nick 2121wedwe|@Google lew 3,100,2124.1@@Tesla Ltd.,200,1000.1@@SpaceX,23,432.1@|factory";
 *  passLineFromCapitalistTXT(line);
 *  to save:
 *  makeSingleStockEntry("nick", singleUserStocks_v);
 *
 ****************************************************************/


#include "stockreadandsave.h"
#include <iostream>
#include <vector>
StockReadAndSave::StockReadAndSave(QObject *parent) : QObject(parent)
{    
}

std::vector<std::vector<std::string>> StockReadAndSave::getStockVec(std::string& line)
{
    std::cout << "StockReadAndSave::getStockVec" << std::endl;
    passLineFromCapitalistTXT(line);
    std::string nick = findNick(line);
    return singleUserStocks_v;
}

StockReadAndSave::~StockReadAndSave()
{
    std::cout << "~StockReadAndSave()" << std::endl;
    //singleUserStocks_v.clear();
}

//========================== READING from CapitalistTXT and changign into Variables ======//:

std::string StockReadAndSave::separateBy_vertSign(std::string& allUserLine)
{
    //gets whats inside | | in "nick|@Google lew 3,100,2124.1@@Tesla Ltd.,200,1000.1@@SpaceX,23,432.1@|factory";
    std::string core;
    int pos = 0;
    int pos2 = 0;
    if((pos = allUserLine.find_first_of("|"))!=std::string::npos &&
        (pos2 = allUserLine.find_last_of("|"))!=std::string::npos )
    {
        core = allUserLine.substr(pos+1, pos2 - pos);
        return core;
    }
    else
        return "";

}

void StockReadAndSave::separateByComas(std::string& extractedEntry)
{
    //extractedEntry here is like "Google lew 3,1001,2124.1";
    int pos_1 = 0;
    int pos_2 = 0;
    int pos_3 = 0;

    std::string nameStock = extractedEntry.substr(0,  pos_1 = extractedEntry.find(","));
    std::cout << "nameStock: " << nameStock << std::endl;
    std::string amount = extractedEntry.substr(pos_1+1,  (pos_2 = extractedEntry.find(",", pos_1+1)) - pos_1 -1);
    std::cout << "amount: " << amount << std::endl;
    std::string price = extractedEntry.substr(pos_2+1, extractedEntry.size() - pos_2+1);
    std::cout << "price: " << price << std::endl;

    std::cout << "------------------------" << std::endl;
    singleUserStocks_v.push_back({nameStock, amount, price});

    //AllUsersStocks_v.push_back(singleUserStocks_v);
}

void StockReadAndSave::separateByMonkey(std::string& colectiveLine)
{
    //gets whats inside @ and @ in "@Google lew 3,100,2124.1@@Tesla Ltd.,200,1000.1@";
    int pos  = 0;
    int pos2 = 0;
    int pos3 = 0;

    std::string temp;
    while (
        (pos  = colectiveLine.find("@", pos3))!=std::string::npos &&
        (pos2 = colectiveLine.find("@", ++pos))!=std::string::npos )
    {
        //std::cout << "pos: " << pos << " pos2: " << pos2 << std::endl;
        temp = colectiveLine.substr(pos , pos2 - pos);
        //std::cout << "temp: " << temp  << std::endl;
        pos3 = pos2 +1;
        separateByComas(temp);
    }
}

std::string StockReadAndSave::findNick(std::string& fullUSerLine)
{
    std::string nick = fullUSerLine.substr(0, fullUSerLine.find(" "));
    return nick;
}

void StockReadAndSave::passLineFromCapitalistTXT(std::string& line)
{
    std::string nick = findNick(line);
    std::cout << "nick: " << nick << std::endl;
    std::string nameStock;
    std::string amount;
    std::string price;

    std::string core;
    core = separateBy_vertSign(line);
    if(core.size()!= 0)
        separateByMonkey(core);

    //std::cout << "-- " << usersStocks_v[1][1] << std::endl; // = 200
}
//========================== Entry new imput to save:

std::string StockReadAndSave::embraceAllStocks(std::string jointEntry)
{
    // is to be line this: "nick 2121wedwe|@Google lew 3,100,2124.1@@Tesla Ltd.,200,1000.1@@SpaceX,23,432.1@|factory";
    jointEntry = "|" + jointEntry + "|";
    std::cout << "jointEntry: " << jointEntry << std::endl;
    return jointEntry;
}


std::string StockReadAndSave::makeSingleStockEntry(std::vector<std::vector<std::string>> singleUserStocks_v)
{
    // is to be line this: "nick 2121wedwe|@Google lew 3,100,2124.1@@Tesla Ltd.,200,1000.1@@SpaceX,23,432.1@|factory";

    std::string nameStock ;
    std::string amount;
    std::string price;
    std::string jointEntry;


    for (int i = 0; i < singleUserStocks_v.size(); ++i) {
        nameStock   = "@" + singleUserStocks_v[i][0];
        amount      = "," + singleUserStocks_v[i][1] + ",";
        price       = singleUserStocks_v[i][2] + "@";
        jointEntry += nameStock + amount + price;
    }


    std::cout << "jointEntry: " << jointEntry << std::endl;
    //embraceAllStocks(jointEntry);
    return jointEntry;
}

//========================== SAVING TO CapitalistTXT and changign into Variables ======//
