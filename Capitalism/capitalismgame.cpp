#include "capitalismgame.h"
#include <iostream>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <sstream>
#include <map>
#include <cstdlib>
#include <utility>
#include <iomanip>
#include <sstream>

//10517.54 f demon

CapitalismGame::CapitalismGame(QObject *parent, AntiFlood* antiFlood_, QMutex *mutex_) : QObject(parent)
{
    std::cout << termcolor::on_red << "CapitalismGame" << termcolor::reset << std::endl;

    kasyno_is = false;
    assetsSummedUp =0.f;
    std::cout  << "CapitalismGame end of ctor\n";
}

CapitalismGame::~CapitalismGame()
{
    // delete listOfCapitalists_v;
    std::cout << "DESTRUCTOR CAPITALIST" << std::endl;
}

void CapitalismGame::run(QString data_, CapitalismStorage *capitalismStorage_)
{
    std::cout << termcolor::yellow<< "CapitalismGame::run" << termcolor::reset<< std::endl;

    data = data_;
    capitalismStorage =  capitalismStorage_; //error
    if(getNickAndRoom() == false)  //return if room or nick are empty
        return;
    else{
        checkKeywords();
        std::cout << "CapitalismGame::run else{ checkKeywords(); ";
    }
    //earningMoney(); // give user money for their activity
}

void CapitalismGame::earningMoney()
{

    if(extract_NickAndRoom.extractText(data).size()>= 100) //if users text is 95 signs long
    {
        capitalismStorage->addMoney(nick, 0); ///earning turn off
        earningCounter++;
        std::cout << termcolor::green << "earningCounter++: " << earningCounter << termcolor::reset<< std::endl;
        if(earningCounter>=5){
            capitalismStorage->saveCapitalistsToFile();
            std::cout << termcolor::green << "capitalismStorage->saveCapitalistsToFile();"  << termcolor::reset<< std::endl;
        }
    }
}

void CapitalismGame::checkKeywords()
{
    std::cout << "CapitalismGame::checkKeywords() DATA: " << data.toStdString() << "\n";
    if(data.size() <6) return; //!kup x - 6 smallest

    Extract_NickAndRoom e;
    std::string cleaned_command;
    if(data.contains("!info "))
        data.replace("!info ", "!infou "); // so i can get all list with "!info" and users info with "!info nick"
    if(data.contains("!kasyno "))
        data.replace("!kasyno ", "!kasynou "); // so i can get all list with "!info" and users info with "!info nick"

    cleaned_command = e.extractCleanedCommandWithoutN(data); //input is "!give nick 100" but cleaned command is "!give" so  nick 100 is cut off for now

    std::cout << "CLEANED COMMAND: " << cleaned_command << std::endl;
    std::map<std::string,int>mapa;
    mapa["!info"]   = 1;

    if(cleaned_command.size() ==0) return; //its past "mapa["!info"]   = 1;" so mapa["!info"]   = 1; doesnt need command

    mapa["!infou"]  = 2; // but  the actual command in chatr is "!info nick"
    mapa["!daj---"]   = 3;
    mapa["!kasyno---"] = 4; /// "---" do wylaczania opcji
    mapa["!kasynou---"] = 5;
    mapa["!awansuj"] = 6;
    mapa["!kup"] = 7;
    mapa["!sprzedaj"] = 8;
    mapa["!sprawdz"] = 9;
    mapa["!zasoby"] = 10; //for now does nothing - its aimed to check how much money user has, including the value of all stocks
    switch (mapa[cleaned_command]) {

    case 1: //"!info"
    {
        std::cout << "BUUUUUUUUUUUM1" << std::endl;
        information();
        return;
    }

    case 2:  // "!info "
    {
        std::cout << "BUUUUUUUUUUUM2" << std::endl;
        if(!atomizeCommand(data, cleaned_command)) //from here we get commandElements_v
            return;

        std::string nick = commandElements_v[0];
        std::cout << "BUUUUUUUUUUUM2 nick:: " << nick << std::endl;
        if(capitalismStorage->determineUser(nick)) //if someone checks for nick that is not present in Storage::capitalists_v: - then create such capitalsit and give 0 money so its 2nd way of creatring a capitalist (after !give newNick 100)
        {
            doPosting(informationOfUser(nick));
            doPosting(informationOfUserStocks(nick));
        }
        else
        {
            capitalismStorage->createEntry(nick, 0.0);
            doPosting(informationOfUser(nick));
            std::cout << "make new capitalist from !infoU newnick" << std::endl;
        }
        return;
    }


    case 3: //"!give" //must                TODO: check if registered
    {
        std::cout << "!give" << std::endl;

        std::cout << "BUUUUUUUUUUUM3" << std::endl;
        if (!atomizeCommand(data, cleaned_command)) //from here we get commandElements_v
            return;
        //std::string post = commandElements_v[0] + " " + commandElements_v[1];
        std::string post;
        std::string nick;
        std::string nick_ofGiver = extract_NickAndRoom.extractNick(data);
        double giversMoney = capitalismStorage->getUserFromNick(nick_ofGiver)->getMoney();
        std::cout << "GIVER: " << nick_ofGiver << std::endl;

        if( checkIfNumber(commandElements_v[1])  && commandElements_v.size()!=0 &&  commandElements_v[0].size()!=0 ) //check if what we give is a number |  commandElements_v.size()!=0 - cause the vector itself cant be empty either
        {
            /* when someone "!give nick 0" then commandElements_v[1] becomes 0 and the whole expression checkIfNumber(commandElements_v[1] also - 0
             * thats why we cannot return it directly as int from checkIfNumber() ) */


            double money =  std::stod(commandElements_v[1]) ;
            nick  = commandElements_v[0];
            std::cout << "ZIIIIIIIIIIIII nick is: " << nick << " and money is: " << money << std::endl;
            if(giversMoney>=money && money>0) //if giver has more of the same money as they give to the receiver - then proceed
            {
                capitalismStorage->addMoney(nick, money);
                capitalismStorage->addMoney(nick_ofGiver, -money);
                doPosting(informationOfUser(nick));            //display !info nick after giving
                doPosting(informationOfUser(nick_ofGiver));    //display !info giver's nick after givin
                capitalismStorage->saveCapitalistsToFile();
            }
            else if(money<0)
            {
                doPosting("nie wolno oszukiwać i odejmować komuś kasę, Łosiu");
            }
            else
                doPosting("dałeś figę z makiem, Łosiu");
        }
        return;
    }
    case 4: // "kasyno!" - starts kasyno
    {
        std::cout << "BUUUUUUUUUM4" << std::endl;
        if(kasyno_is == true)
            return;
        nick.clear();
        startKasyno();
        return;
    }

    case 5: // "!kasyno " - starts kasyno and takes bet
    {


        std::cout << "BUUUUUUUUUUUM 5" << std::endl;
        if(!atomizeCommand(data, cleaned_command)) //from here we get commandElements_v
            return;
        std::cout << "BUUUUUUUUUUUM5 nick:: " << nick << std::endl;
        std::cout << termcolor::on_red << "commandElements_v[0]:|"<< commandElements_v[0] << "|" << termcolor::reset<< std::endl;
        std::cout << termcolor::red << "kasyno amount: " << commandElements_v[0] << termcolor::reset<< std::endl;

        std::cout << termcolor::on_green << "QThread.currentThread().currentThreadId() in CapitalismGame: " <<  QThread::currentThread()->currentThreadId() << termcolor::reset<< std::endl;



        if(!checkIfNumber(commandElements_v[0])) //check if bet money is a long double
        {
            doPosting("\x03""07" + nick + " co ty robisz? postaw cos" );
            return;
        }

        std::cout << "SIGNAL emit betMoneySignal(pair)" << std::endl;
        long double temp = std::stof(commandElements_v[0]);
        emit betNickSignal(temp, QString::fromStdString(nick)); //betMoney (turned into long double)

        //capitalismKasyno->run(std::stof(commandElements_v[0])); //betMoney (turned into long double)
        return;
    }
    case 6: // "!awansuj " - rank up
    {
        std::cout << "BUUUUUUUUUUUUUUUUUUUUUM 6" << std::endl;
        if (!atomizeCommand(data, cleaned_command)) //from here we get commandElements_v
            return;
        std::string post;
        double risersMoney = 0;
        auto userFromNick = capitalismStorage->getUserFromNick(nick);
        if(userFromNick){
            risersMoney = userFromNick->getMoney();
        }else{
            std::cout << "CapitalismGame case 6, userFromNick == nullptr" << std::endl;
        }

        std::cout << "RISER: " << nick << std::endl;

        if( checkIfNumber(commandElements_v[0])  && commandElements_v.size()!=0 &&  commandElements_v[0].size()!=0 ) //check if what we give is a number |  commandElements_v.size()!=0 - cause the vector itself cant be empty either | check [zero] cause despide !give nick money - money is [1], here is [0]
        {
            /* when someone "!give nick 0" then commandElements_v[1] becomes 0 and the whole expression checkIfNumber(commandElements_v[1] also - 0
             * thats why we cannot return it directly as int from checkIfNumber() ) */
            double money =  std::stof(commandElements_v[0]) ;
            std::cout << "ZIIIIIIIIIIIII nick is: " << nick << " and money is: " << money << std::endl;
            if(risersMoney>=money && money>0) //if riser has more of the same money as they give to rise their social status - then proceed
            {
                capitalismStorage->addRank(nick, 1);
                capitalismStorage->saveCapitalistsToFile();
                capitalismStorage->addMoney(nick, -money);
                doPosting(informationOfUser(nick));            //display !info nick after giving
            }
            else //(money<0)
                doPosting("nie masz tyle srodkow żeby awansować");

        }
        return;
    }
    case 7: // "!kup 1 GOOG"
    {
        std::cout << "!kup stocks" << std::endl;

        std::cout << "BUUUUUUUUUUUM7" << std::endl;
        if (!atomizeCommand(data, cleaned_command) ||  commandElements_v.size() <2 ) //from here we get commandElements_v
            return;
        //std::string post = commandElements_v[0] + " " + commandElements_v[1];
        std::string post;
        std::string stockName = "";
        std::string nick  = extract_NickAndRoom.extractNick(data);

        if(!capitalismStorage->determineUser(nick)) //if someone buys from nick that is not present in Storage::capitalists_v: - then create such capitalsit and give 0 money so its 2nd way of creatring a capitalist (after !give newNick 100)
        {
            capitalismStorage->createEntry(nick, 0.0);
            doPosting(informationOfUser(nick));
            //doPosting(informationOfUserStocks(nick));
            std::cout << "make new capitalist from !infoU newnick" << std::endl;
        }

        std::cout << "nick of Buyer: " << nick << std::endl;
        std::cout << "commandElements_v.size(): " << commandElements_v.size()<< std::endl;
        std::cout << "commandElements_v[0]: " << commandElements_v[0] << std::endl;
        std::cout << "commandElements_v[1]: " << commandElements_v[1] << std::endl; //name


        if(checkIfNumber(commandElements_v[0])  && commandElements_v.size()==2 &&  commandElements_v[1].size()!=0 &&
                !checkIfNumber(commandElements_v[1])) //check if what we give is a number |  commandElements_v.size()!=0 - cause the vector itself cant be empty either
        {   // !checkIfNumber(commandElements_v[1]) 2 rd element must be not a numer - cause its a name of stock
            std::cout << "INSIDE IF" << std::endl;
            int amountBought =  std::stod(commandElements_v[0]) ;
            if(amountBought<=0)
            {
                doPosting("co robisz Łosiu? nie wolno kupowac ułamkowych części akcji ani zera akcji");
                return;
            }

            /* when someone "!give nick 0" then commandElements_v[1] becomes 0 and the whole expression checkIfNumber(commandElements_v[1] also - 0
             * thats why we cannot return it directly as int from checkIfNumber() ) */

            for (int i = 1; i < commandElements_v.size(); ++i) { //stock name can be of multiple words
                if(stockName.size() ==0)
                    stockName+=commandElements_v[i]; // dont add " " before first word
                else {
                    stockName+=" ";
                    stockName+= commandElements_v[i]; //stock name can be of multiple words
                }
            }

            stockMarket = new StockMarket;

            std::vector<std::string> temp = stockMarket->process(stockName);
            delete stockMarket;
            if(temp.size()== 0){
                std::string post = "nie ma takich akcji w sprzedaży, Łosiu";
                doPosting(post);
                return;
            }
            //temp[0] = stock name   temp[1] = price
            double moneyNeeded = amountBought * std::stod(temp[1]);
            double buyersMoney = 0;
            auto userFromNick = capitalismStorage->getUserFromNick(nick);
            if(userFromNick){
                buyersMoney = capitalismStorage->getUserFromNick(nick)->getMoney();
            }else{
                std::cout << "Capitalismgame case 7, userFromNick == nullptor" << "\n";
                return;
            }

            std::cout << "ZIIIIIIIIIIIII nick is: " << nick << " and amount of bought stock is: " << amountBought << " stockName: " << stockName << std::endl;

            if(buyersMoney>= moneyNeeded && moneyNeeded>0) //if buyer has the money to buy - then proceed
            {
                stockMarket = new StockMarket;
                auto userFromNick = capitalismStorage->getUserFromNick(nick);
                if(userFromNick){
                    userFromNick->addNewStock(stockName, amountBought, temp[1]); //temp[1] = actual price
                    capitalismStorage->addMoney(nick , -moneyNeeded);
                }else{
                    std::cout << "Capitalismgame case 7: userFromNick is nullptr" << "\n";
                    return;
                }



                std::cout << termcolor::red << "temp[1]: |" << temp[1] << "|" << termcolor::reset<< std::endl;
                doPosting("kupiles " + round_float(amountBought) + " " + temp[0] + " po cenie jednostkowej: " + temp[1] );
                doPosting(informationOfUser(nick));    //display !info giver's nick after givin
                doPosting(informationOfUserStocks(nick));
                capitalismStorage->saveCapitalistsToFile();
            }
            else
            {
                doPosting("nie masz tyle srodkow zeby to kupic, Łosiu");
            }

        }
        return;
    }
    case 8: // "!sprzedaj 1 GOOG"
    {
        std::cout << "!sprzedaj stocks" << std::endl;

        std::cout << "BUUUUUUUUUUUM8" << std::endl;
        if (!atomizeCommand(data, cleaned_command) ||  commandElements_v.size() <2 ){ //from here we get commandElements_v
            doPosting("co ty wpisujesz, zła komenda, Łosiu");
            return;
        }
        //std::string post = commandElements_v[0] + " " + commandElements_v[1];
        std::string post;
        std::string stockName = "";
        std::string nick  = extract_NickAndRoom.extractNick(data);

        if(capitalismStorage->getUserFromNick(nick)->getStocks().size()==0 ) //if someone buys from nick that is not present in Storage::capitalists_v: - then create such capitalsit and give 0 money so its 2nd way of creatring a capitalist (after !give newNick 100)
        {
            std::string post = "nie masz żadnych akcji na sprzedaż, Łosiu";
            doPosting(post);
            return;
        }

        std::cout << "nick of Buyer: " << nick << std::endl;
        std::cout << "commandElements_v[0]: " << commandElements_v[0] << std::endl;
        std::cout << "commandElements_v[1]: " << commandElements_v[1] << std::endl;


        if(checkIfNumber(commandElements_v[0])  && commandElements_v.size()>=2 &&  commandElements_v[1].size()!=0 &&
                !checkIfNumber(commandElements_v[1])) //check if what we give is a number |  commandElements_v.size()!=0 - cause the vector itself cant be empty either
        {   // !checkIfNumber(commandElements_v[1]) 2 rd element must be not a numer - cause its a name of stock
            std::cout << "INSIDE IF" << std::endl;
            int amountSold =  std::stod(commandElements_v[0]) ;
            if(amountSold<=0)
            {
                doPosting("co robisz Łosiu? nie wolno kupowac ułamkowych części akcji ani zera akcji");
                return;
            }

            /* when someone "!give nick 0" then commandElements_v[1] becomes 0 and the whole expression checkIfNumber(commandElements_v[1] also - 0
             * thats why we cannot return it directly as int from checkIfNumber() ) */

            for (int i = 1; i < commandElements_v.size(); ++i) { //stock name can be of multiple words
                if(stockName.size() ==0)
                    stockName+=commandElements_v[i]; // dont add " " before first word
                else {
                    stockName+=" ";
                    stockName+= commandElements_v[i]; //stock name can be of multiple words
                }
            }

            stockMarket = new StockMarket;

            std::vector<std::string> temp = stockMarket->process(stockName);
            delete stockMarket;
            if(temp.size()== 0){
                std::string post = "nie ma takich akcji w sprzedaży, Łosiu";
                doPosting(post);
                return;
            }
            //temp[0] = stock name   temp[1] = price
            //TODO:
            std::string postGain = capitalismStorage->getUserFromNick(nick)->sellStock(stockName, amountSold, std::stod(temp[1]));

            //std::cout << "ZIIIIIIIIIIIII nick is: " << nick << " and amount of sold stock is: " << amountSold << " stockName: " << stockName << std::endl;



            std::cout << termcolor::red << "temp[1]: |" << temp[1] << "|" << termcolor::reset<< std::endl;
            doPosting("sprzedałes " + round_float(amountSold) + " " + temp[0] + " po cenie jednostkowej: " + temp[1] );
            doPosting(postGain);

            //selling stocks (even if the user lost money on the trade due to price change), makes you user's wallet gain money:
//            float gainToWalletFromSellingAllStocks = amountSold  * std::stod(temp[1]);
//            capitalismStorage->addMoney(nick, std::abs(gainToWalletFromSellingAllStocks));
            capitalismStorage->saveCapitalistsToFile();
            //doPosting(informationOfUser(nick));    //display !info giver's nick after givin
            //doPosting(informationOfUserStocks(nick));


        }

        return;
    }

    case 9: // "!sprawdz GOOG"
    {
        std::cout << "BUUUUUUUUUUUM 9" << std::endl;
        if (!atomizeCommand(data, cleaned_command)) //from here we get commandElements_v
            return;
        std::string post;
        std::string stockName = "";
        std::string nick  = extract_NickAndRoom.extractNick(data);

        std::cout << "nick of Buyer: " << nick << std::endl;
        std::cout << "commandElements_v[0]: " << commandElements_v[0] << std::endl; // here its stock name

        if(commandElements_v.size()==0 || commandElements_v[0].size()==0 || checkIfNumber(commandElements_v[0]))
            return;


        for (int i = 0; i < commandElements_v.size(); ++i) { //stock name can be of multiple words
            if(stockName.size() ==0)
                stockName+=commandElements_v[i]; // dont add " " before first word
            else {
                stockName+=" ";
                stockName+= commandElements_v[i]; //stock name can be of multiple words
            }
        }
        std::cout << termcolor::red << "stockName: " << stockName << termcolor::reset<< std::endl;
        stockMarket = new StockMarket;

        std::vector<std::string> temp = stockMarket->process(stockName);
        delete stockMarket;
        if(temp.size()== 0){
            std::string post = "nie ma takich akcji w sprzedaży, Łosiu";
            doPosting(post);
            return;
        }
        //temp[0] = stock name   temp[1] = price
        std::string postGain;
        if(capitalismStorage){
            std::cout << "Capitalismgame case 9: nick: " << nick  << "\n";
            auto userFromNick = capitalismStorage->getUserFromNick(nick);
            if(userFromNick)
                postGain = userFromNick->checkStock(stockName, std::stod(temp[1]));
            else{
                std::cout << "Capitalismgame case 9: userFromNick: is nullptr " << "\n";
                //so we need to add current "command caller" (that is user), as new capitalist, as he s non existent:

                capitalismStorage->createEntry(nick, 0.0);
//                doPosting(informationOfUser(nick));
                doPosting(nick + " nie ma cię na liście kapitalistów, dodam cię");
                //doPosting(informationOfUserStocks(nick));
                std::cout << "make new capitalist from !infoU newnick" << std::endl;
                capitalismStorage->saveCapitalistsToFile();
            }
        }
        else
            std::cout << termcolor::blue << "CapitalismGame::checkKeywords(): capitalismStorage is nullptr" <<  termcolor::reset<< std::endl;
        std::cout << "ZIIIIIIIIIIIII nick is: " << nick << " and amount of stock is: " << 1 << " stockName: " << stockName << std::endl;

        stockMarket = new StockMarket;

        std::cout << termcolor::red << "temp[1]: |" << temp[1] << "|" << termcolor::reset<< std::endl; //cena
        doPosting("akcje "  + temp[0] + " są w cenie jednostkowej: " + temp[1] );
        doPosting(postGain);

        return;
    }
    case 10:  // "!wartosc " - does nothing for now !!
    {
        std::cout << "BUUUUUUUUUUUM10" << std::endl;
        if(!atomizeCommand(data, cleaned_command)) //from here we get commandElements_v
            return;

        std::string nick = commandElements_v[0];
        std::cout << "BUUUUUUUUUUUM10 nick:: " << nick << std::endl;

        if(capitalismStorage->determineUser(nick)) //if someone checks for nick that is not present in Storage::capitalists_v: - then create such capitalsit and give 0 money so its 2nd way of creatring a capitalist (after !give newNick 100)
        {
            std::cout << "" << "\n";
            sumUpUsersStockValue(nick);
            nick.clear();
            commandElements_v.clear();
        }
        else
        {
            capitalismStorage->createEntry(nick, 0.0);
            doPosting(informationOfUser(nick));
            std::cout << "make new capitalist from !infoU newnick" << std::endl;
        }
        return;
    }
    default:{
        std::cout << termcolor::red << "LIPA" << termcolor::reset<< std::endl;
        std::cout << "nick: " << nick << "|\n";
        std::cout << "room: " << room << "|\n";
        std::cout << "cleaned_command: " << cleaned_command << "|\n";
        return;
    }
    }
}

double CapitalismGame::checkActualStockPrize(std::string stockName)
{
    std::cout << termcolor::red << "stockName: " << stockName << termcolor::reset<< std::endl;
    stockMarket = new StockMarket;

    std::vector<std::string> temp = stockMarket->process(stockName);
    delete stockMarket;
    if(temp.size()== 0){
        std::string post = "Error, Łosiu, części akcji nie da sie sprawdzić, coś sie zepsulo, spróbój później";
        doPosting(post);
        return 0.f;
    }

    else return std::stof(temp[1]); // return price of certain stock
}

void CapitalismGame::sumUpUsersStockValue(std::string nick)
{

    std::cout << termcolor::green << "sumUpUsersStockValue(): " <<  nick << termcolor::reset<< std::endl;
    std::vector<std::vector<std::string>> singleUserStocks_v = capitalismStorage->getUserFromNick(nick)->getStocks();
    if(singleUserStocks_v.size() == 0){
        doPosting("nie masz akcji, sprzwdź gotówkę przez komendę !info nick");
        return;
    }

    double valueOfUsersStocks = 0.f;

    for (int i = 0; i < singleUserStocks_v.size(); ++i) {
        double prizeOfGivenStock = checkActualStockPrize(singleUserStocks_v[i][0]);
        std::cout << termcolor::red << "prizeOfGivenStock: " << prizeOfGivenStock << termcolor::reset<< std::endl;
        valueOfUsersStocks += prizeOfGivenStock * std::stoi(singleUserStocks_v[i][1]);
    }

    double valueOfUsersMoney = capitalismStorage->getUserFromNick(nick)->getMoney(); //add poseesed money to sumed up falue of your stocks
    //==========

    assetsSummedUp = valueOfUsersStocks + valueOfUsersMoney;

    std::cout << termcolor::red << "valueOfUsersStocks: " << valueOfUsersStocks << termcolor::reset<< std::endl;
    std::cout << termcolor::red << "valueOfUsersMoney: " << valueOfUsersMoney << termcolor::reset<< std::endl;
    std::cout << termcolor::red << "assetsSummedUp: " << assetsSummedUp << termcolor::reset<< std::endl;

    std::string post = nick + " jest warty: " + round_float(assetsSummedUp)  + " coinów, ma " + round_float(valueOfUsersStocks) + " w akcjach i " + round_float(valueOfUsersMoney) + " w gotówce";
    //std::cout << termcolor::green << "temp[1]: " << temp[1] << termcolor::reset<< std::endl;
    doPosting(post);
    assetsSummedUp = 0.f;

    return;
}

void CapitalismGame::startKasyno(double betMoney_, QString nick)
{
    std::cout << "startKasyno" << std::endl;
    kasyno_is = true; // turning on kasyno
    std::cout << termcolor::on_green << "QThread.currentThread().currentThreadId() in game: " <<  QThread::currentThread()->currentThreadId() << termcolor::reset<< std::endl;
    //capitalismKasyno in separate thread:
    kasyno = new QThread;
    capitalismKasyno = new CapitalismKasyno(kasyno,nick, room, data, betMoney_, capitalismStorage, this);
    capitalismKasyno->moveToThread(kasyno);
    kasyno->start();
    return;
}


bool CapitalismGame::atomizeCommand(QString data_, std::string command) // gets xx after "!capita xxx" and turns into vector
{
    std::cout << "atomizeCommand()" << std::endl;
    std::cout << "data in atomize: " << data_.toStdString() << "\n";
    std::cout << "command in atomize: " << command << std::endl;
    commandElements_v = extract_NickAndRoom.extractCommandElements(data_, command);
    if (commandElements_v.size() == 0)
    {
        std::cout << "commandElements_v  is EMPTY" << std::endl;
        return false;
    }
    else
    {
        std::cout << "commandElements_v[0] " << commandElements_v[0]<< std::endl;
        return true;
    }
}

void CapitalismGame::information()
{
    std::cout << "CapitalismGame::information(): " << std::endl;

    //    for (auto i: listOfCapitalists_v) {

    //        doPosting(extract_NickAndRoom.reduceZerosInFloatString(i)  );
    //        QThread::msleep(750);
    //    }

    std::string info = "\x03""13(akcje nieujęte, dla szczegółów wpisz !info \"nick\")";
    for (auto i  : capitalismStorage->get_capitalists_v()) {
        if(i.getMoney()==10000) //skips info when user has exactly the starting point money
            continue;
        std::string line = "";
        line += i.getNick() ;
        line += " ma ";
        line += std::to_string(i.getMoney());
        line += " coinow, a pozycję społeczną ";
        line += std::to_string(i.getRank());

        //line += i.getFactory();
        std::cout << "LINE: " << line << std::endl;
        doPosting(extract_NickAndRoom.reduceZerosInFloatString(line));
        QThread::msleep(750);
    }
    doPosting(info);

}

std::string CapitalismGame::informationOfUser(std::string nick)
{
    std::cout << "CapitalismGame::informationOfUser(): " << std::endl;
    double money = capitalismStorage->getUserFromNick(nick)->getMoney();
    int rank = capitalismStorage->getUserFromNick(nick)->getRank();
    std::string post;
    if(money == 0)
        post = nick + " jest goly, ma: " + round_float(money ) + " coinow, a pozycję społeczną " + std::to_string(rank);
    else
        post = nick + " ma: " + round_float(money) + " coinów, a pozycję społeczną " + std::to_string(rank); //passing money but reducing extra zeroes
    std::cout << "POSTPOST POST " << post << std::endl;
    return post;

}
std::string CapitalismGame::informationOfUserStocks(std::string nick)
{
    std::cout << "CapitalismGame::informationOfUserStocks(): " << std::endl;
    std::vector<std::vector<std::string>> singleUserStocks_v = capitalismStorage->getUserFromNick(nick)->getStocks();
    if(singleUserStocks_v.size() == 0)
        return "";

    std::string post = "akcje: ";
    for (int i = 0; i < singleUserStocks_v.size(); ++i) {
        post+= "\x03\x34 " + singleUserStocks_v[i][0] + "\x03 w ilości:";
        post+= "\x03\x35 " + singleUserStocks_v[i][1] + "\x03 kupione po cenie za akcję:";
        post+= "\x03\x35 " + singleUserStocks_v[i][2] + " coinów \x03\x37||\x03\ " ;
    }

    std::cout << "POSTPOST POST " << post << std::endl;
    return post;

}

//if( checkIfNumber(commandElements_v[1]) &&  commandElements_v[0].size()!=0)
bool CapitalismGame::checkIfNumber(std::string& s)
{
    std::string::const_iterator it = s.begin();

    int index = 0;
    std::replace(s.begin(), s.end(),  ',', '.');

    for (it = s.begin(); it!= s.end(); ++it) {
        if(std::isdigit(*it) || *it== ',' || *it =='.' || *it =='-')
        {
            if(index >0 && *it == '-')
                return false;
            if(index == 0 && ( (*it == '.') || (*it == ',') ) )
                return false;
            if(!std::isdigit(s[s.size()-1]))
                return false;
            std::cout << *it << " index: " << index << std::endl;
        }
        else
            return false;
        index++;
    }
    return true;
}


bool CapitalismGame::getNickAndRoom()
{
    std::cout << "Capitalism checkPartPresence()" << std::endl;

    Extract_NickAndRoom e;
    nick = e.extractNick(data);
    room = e.extractRoom(data);
    std::cout << "Capitalism nick: " << nick << "nick.size(): " << nick.size() << std::endl;
    std::cout << "Capitalism room: " << room << "room.size(): " << room.size() << std::endl;
    std::cout << "CapitalismGame::getNickAndRoom()" << std::endl;
    if(nick.size() == 0 || room.size() < 2)
        return false;
    else
        return true;

    //        QTimer::singleShot(5000 , this, SLOT(postInvite()));

}



void CapitalismGame::doPosting(const std::string& newsText)
{
    //    mutex->lock();
    std::cout << "CapitalismGame::doPosting()" << std::endl;
    std::string lineToPost;
    lineToPost = "PRIVMSG " + room + " :" +  newsText + "\r\n\ ";

    //    const char *send = lineToPost.c_str();

    QByteArray post = QByteArray::fromStdString(lineToPost);
    std::cout << "Game:: sendiiiiiing: " <<  lineToPost << std::endl;

    emit postSignal(post);
}

void CapitalismGame::doPosting2(QByteArray post) //post << lineToPost << nick << room;
{
    //    mutex->lock();
    if(post.size() == 0)
        return;

    std::cout << "CapitalismGame::doPosting()" << std::endl;

    std::cout << "Game:: sendiiiiiing: " << post.toStdString() << std::endl;

    //    QByteArray poster = QByteArray::fromStdString(post);


    emit postSignal(post);


    //    mSocket->write(post);
    //    mSocket->flush();
    //    mSocket->waitForBytesWritten();
    //    mutex->unlock();
}


std::string CapitalismGame::round_float(double money)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << money;
    return ss.str();
}

bool CapitalismGame::get_kasyno_is()
{
    return kasyno_is;
}

void CapitalismGame::fineCapitalistFlooder(std::vector<std::string> flooder_v)
{
    std::cout << termcolor::blue << "fineCapitalistFlooder: " << flooder_v[0] << " " << flooder_v[1] << termcolor::reset<< std::endl;
    std::string nick = flooder_v[0];
    long double fine = -1000;
    capitalismStorage->addMoney(flooder_v[0], fine);

    std::string post = "\x03""13" + nick + " za flood płacisz grzywnę w wysokości " + round_float(fine)  + " coinów";
    doPosting(post);
}

void CapitalismGame::threadFinished()
{
    std::cout << termcolor::on_blue << "threadFinished()" << termcolor::reset<< std::endl;
    if(kasyno->isFinished())
    {
        kasyno_is = false;
        std::cout << termcolor::red << "kasyno finished" << termcolor::reset<< std::endl;
        delete capitalismKasyno;
    }
}
