#include "capitalismstorage.h"
#include "loadlogs.h"
#include "qcoreapplication.h"
#include <sstream>
#include <QCoreApplication>
CapitalismStorage::CapitalismStorage(QObject *parent, LoadLogs *loadLogs_, CapitalismGame* capitalismGame_): QObject(parent)
{
    loadLogs  = loadLogs_;
    capitalismGame = capitalismGame_;
    directory = QCoreApplication::applicationDirPath();
}

//this function fires at the beginning of the program - to get data from file:
void CapitalismStorage::attachAssetsToUser()
{
    std::cout << "CAPITALISM STORAGE attachAssetsToUser()" << std::endl;
    listOfCapitalists_v = &loadLogs->get_listOfCapitalists(); //get what was loaded from file

    for (auto i =0; i < listOfCapitalists_v->size(); i++){
        std::stringstream ss;       // to separate "nick 100 fabryka" from vector
        ss << listOfCapitalists_v->at(i) ;                                       //"kiki 100 fabryka"
        std::string temp;
        std::vector<std::string> temp_v;

        while(ss>>temp && temp.find("|")==std::string::npos) //temp.find("|") so stock info wont be split into singular words by sstream
        {
            std::cout << "temp: " << temp << std::endl;
            temp_v.push_back(temp);
        }
        temp_v.push_back(listOfCapitalists_v->at(i)); //pushing whole text from txt for parsing by socketReadAndSave class
        users_andSingularAssets_v.push_back(temp_v);

    } //for

    std::cout << termcolor::on_cyan << "users_andSingularAssets_v.push_back(temp_v); DONE" << termcolor::reset<< std::endl;

    //std::vector< std::vector<std::string> > users_andSingularAssets_v;

    for (int i = 0; i < users_andSingularAssets_v.size(); ++i) {
        std::string nick;
        std::string money;
        std::string rank;
        std::string factory;
        std::string stockstsAllInLine;
        size_t sizeOfVec = users_andSingularAssets_v[i].size();
        int index =0; // so we wont access vectory beyond its size-1, which would cause runtime error:
        if(index < sizeOfVec)
            nick    = users_andSingularAssets_v[i][index++];
        if(index < sizeOfVec)
            money   = users_andSingularAssets_v[i][index++];
        if(index < sizeOfVec)
            rank   = users_andSingularAssets_v[i][index++];
        if(index < sizeOfVec)
            stockstsAllInLine   = users_andSingularAssets_v[i][index++]; // gets stocks as they are in txt so we can parse in stocksReadAndSave
        //if(index < sizeOfVec)
            //factory = users_andSingularAssets_v[i][index++]; // here was ERROR cause of accessing out of bound (we accessed factory from file that didnt exist)

        //stock stuff:
        std::cout << termcolor::red << "stockstsAllInLine: " << stockstsAllInLine << termcolor::reset<< std::endl;
        //now get Stocks from whole txt enter of user (wich is listOfCapitalists_v->at(i)):
        StockReadAndSave stockReadAndSave;
        std::vector<std::vector<std::string>> singleUserStocks_v = stockReadAndSave.getStockVec(stockstsAllInLine);
        //stock stuff end;
        std::cout << termcolor::on_yellow << "nick and singleUserStocks_v.size: "<< nick << " " << singleUserStocks_v.size() << termcolor::reset<< std::endl;
        //Capital is created and users get the variables:

        Capital c(this) ;
        c.setNick(nick);
        c.setMoney(std::stod(money));
        c.setRank(std::stoi(rank));
        c.setAllStocks(singleUserStocks_v);
        //c.setFactory(factory);
        capitalists_v.push_back(c);
    }
    std::cout << termcolor::on_cyan << "Loading finished - attachAssetsToUser() DONE" << termcolor::reset<< std::endl;
    std::cout << "Loading finished - attachAssetsToUser()" << std::endl;
}

std::vector<std::string> CapitalismStorage::get_infoAllCapitalists()
{
    /* iterate through std::vector<Capital> capitalists_v and but all its variables
    into one line so it can be printed later */
    std::vector<std::string> temp_v;
    for (int i = 0; i < capitalists_v.size(); ++i)
    {
        std::string line;
        line += capitalists_v[i].getNick() ;
        line += " ";
        line += std::to_string(capitalists_v[i].getMoney());
        line += " ";
        line += std::to_string(capitalists_v[i].getRank());
        line += " ";
        line += capitalists_v[i].getFactory();
        std::cout << "LINE: " << line << std::endl;
        temp_v.push_back(line);
    }
    return temp_v;
}

void CapitalismStorage::addMoney(std::string nick, double money)
{
    std::cout << "CapitalismStorage::addMoney" << std::endl;
    //Capital* capTemp = &determineUser(nick);
    if(determineUser(nick))  //if nick already present in capitalists_v:
    {
        capitalTemp->setMoney(money);
        std::cout << "CapitalismStorage::addMoney " << capitalTemp->getMoney() << std::endl;
    }
    else //create new entry in capitalists_v:
        createEntry(nick, money);
}

void CapitalismStorage::addRank(std::string nick, int rank)
{
    std::cout << "CapitalismStorage::addRank" << std::endl;
    if(determineUser(nick))  //if nick already present in capitalists_v:
    {
        capitalTemp->setRank(rank);
        std::cout << "RAWRRRR " << capitalTemp->getRank() << std::endl;
    }
}

void CapitalismStorage::addStock(std::string nick, double amountBought, std::string stockName)
{
    std::cout << "CapitalismStorage::addStock" << std::endl;
}

std::vector<Capital> &CapitalismStorage::get_capitalists_v()
{
    return capitalists_v;
}

void CapitalismStorage::createEntry(std::string nick, double money=0.0) // in capitalists_v
{
    std::cout << "createEntry" << std::endl;
    Capital c;
    c.setNick(nick);
    c.setMoney(10000);
    //c.setFactory(factory);
    capitalists_v.push_back(c);
    std::cout << "RAWRRRR else" << c.getMoney() << std::endl;
}


bool CapitalismStorage::determineUser(const std::string nick)
{
    std::cout << "CapitalismStorage::determineUser: " << capitalists_v.size() << std::endl;

    for (int i = 0; i < capitalists_v.size(); ++i)
    {
        if(nick == capitalists_v[i].getNick())
        {
            std::cout << "capitalists_v[i]- nick: " << capitalists_v[i].getNick() << std::endl;
            capitalTemp = &capitalists_v[i]; // its like: return capitalists_v[i];
            return true; // nick present
        }
    }

    std::cout << "EMPTY in determineUser" << std::endl;
    //if nick not present in capitalistlist_v - create new entry in capitalists_v
    return false; // nick not present
}

//TODO
Capital *CapitalismStorage::getUserFromNick(std::string nick)
{
    std::cout << "Capital &CapitalismStorage::getUserFromNick(std::string nick) nick: " << nick << "\n";
    if(determineUser(nick) == false){
        std::cout << "Capital &CapitalismStorage::getUserFromNick:  determineUser(nick) == false" << "\n";
        return nullptr;
    }
    return capitalTemp; // it holds info from capitalists_v
}

void CapitalismStorage::saveCapitalistsToFile()
{
    QString relativePath = directory.absoluteFilePath("config/capitalism.txt");
//    std::string address = relativePath.toStdString() + nickFileName + ".txt";

    file.open(relativePath.toStdString(), std::ios::out);
    std::cout << "CapitalismStorage::saveCapitalistsToFile(): relativePath: " << "\n";
    StockReadAndSave stockReadAndSave;

    for (int i = 0; i < get_infoAllCapitalists().size(); ++i) {
        std::string entry1 = capitalists_v[i].getNick() + " " + std::to_string(capitalists_v[i].getMoney()) + " " + std::to_string(capitalists_v[i].getRank()) + " " ;
        std::string entry2;
        std::string wholeEntry;
        if(capitalists_v[i].getStocks().size()!=0)  {
            entry2 = stockReadAndSave.makeSingleStockEntry(capitalists_v[i].getStocks());
            entry2 = stockReadAndSave.embraceAllStocks(entry2);
             wholeEntry = entry1 + entry2;
        }
        else{
            wholeEntry = entry1;
        }
        file << wholeEntry << '\n';
    }
    file.close();
}

void CapitalismStorage::determineLevels()
{
    std::vector<int> levels_v;
    int start = 100;
    for (int i = 0; i < 40 ; ++i) {
        if(start< 10000)
            start = start *2;
        else if (start <6000)
            start = start *1.2;
        else
            start = start *1.123;
        levels_v.push_back(start);
    }
}

CapitalismGame *CapitalismStorage::getCapitalismGamePtr()
{
    return capitalismGame;
}



