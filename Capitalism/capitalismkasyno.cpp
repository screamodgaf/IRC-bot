#include "capitalismkasyno.h"
#include <termcolor.hpp>
#include <iostream>
#include <QDebug>
#include <math.h>
#include <random>
//mutex doesnt help distorted messages in console
CapitalismKasyno::CapitalismKasyno(QThread *kasyno_, QString nick_, std::string room_, QString data_, double betMoney_, CapitalismStorage* capitalismStorage_, CapitalismGame* capitalismGame_)
{
    std::cout << "CapitalismKasyno::CapitalismKasyno" << std::endl;
    kasyno = kasyno_;
//    mutex = mutex_;
    nick = nick_.toStdString();
    room = room_;
    data = data_;
    timer = new QTimer(this);
    capitalismGame = capitalismGame_;
    capitalismStorage = capitalismStorage_;

    connect(kasyno, SIGNAL(started()), this, SLOT(passer()) );
    connect(this, SIGNAL(sendPost(QByteArray)), capitalismGame_, SLOT(doPosting2(QByteArray)));
    connect(kasyno, SIGNAL(finished()), capitalismGame, SLOT(threadFinished()));
    connect(capitalismGame, SIGNAL(betNickSignal(double,QString)) , this, SLOT(run(double,QString)));
    connect(timer, SIGNAL(timeout()), this, SLOT(gameFinished()));
}

void CapitalismKasyno::run(double betMoney_, QString nick_)
{
    std::cout << termcolor::green << "nick: " << nick << termcolor::reset<< std::endl;
    if(betMoney_ ==-1) // dont continue if !kasyno without money
        return;

    nick = nick_.toStdString();
    betMoney = betMoney_;
    mutex->lock();
    std::cout << termcolor::on_green << "run() QThread.currentThread().currentThreadId() in CapitalismKAsyno: " <<  QThread::currentThread()->currentThreadId()<< termcolor::reset<< std::endl;
    mutex->unlock();

    std::string post;

    double assets = capitalismStorage->getUserFromNick(nick)->getMoney();
    //check users assets

    if(betMoney > assets){
        post = "\x03""07" + nick + " nie można obstawiać wiecej, niż masz, Łosiu" ;
        std::string lineToPost = "PRIVMSG " + room + " :" +  post + "\r\n\ ";
        emit sendPost(QByteArray::fromStdString(lineToPost));
        return;
    }

    if (betMoney <= 0){
        post = "\x03""07" + nick + " co ty odstawiasz? postaw cos" ;
        std::string lineToPost = "PRIVMSG " + room + " :" +  post + "\r\n\ ";
        emit sendPost(QByteArray::fromStdString(lineToPost));
        return;
    }
    if (betMoney > 0){
        post = "\x03""07" + nick + " postawiłeś " + capitalismGame->round_float(betMoney) + " coinow";
        std::string lineToPost = "PRIVMSG " + room + " :" +  post + "\r\n\ ";
        emit sendPost(QByteArray::fromStdString(lineToPost));
    }

    timer->setInterval(10000);
    timer->start();

    replaceSameEnteries(playersAndTheirBets_v); //and add new

    auto userFromNick = capitalismStorage->getUserFromNick(nick);
    if(userFromNick)
        userFromNick->setMoney(-betMoney);
    else{
        std::cout << "CapitalismKasyno::run userFromNick == nullptor" << "\n";
        return;
    }




    return;
}



void CapitalismKasyno::passer() // so new thread contains all class and also empty !kasyno without betmoney starts thread
{
    mutex->lock();
    std::cout << termcolor::on_green << "run() QThread.currentThread().currentThreadId() PASSER in CapitalismKAsyno: " <<  QThread::currentThread()->currentThreadId()<< termcolor::reset<< std::endl;
    mutex->unlock();

    std::string post = "\x03""07wejdź do Kasyna i pomnóż swoje coiny! daj komende !kasyno i ilość postawionych coinow, a z póli ze wszystkich zakładów zostana przydzielona kilku lub 1 userowi";
    std::string lineToPost = "PRIVMSG " + room + " :" +  post + "\r\n\ ";
    emit sendPost(QByteArray::fromStdString(lineToPost));
    run(-1, "");
}

// /msg nickserv status queequeg
// /msg nickserv info queequeg

void CapitalismKasyno::replaceSameEnteries(std::vector<std::pair<std::string, double>>& playersAndTheirBets_v)
{
    mutex->lock();
    std::cout << termcolor::green << "replaceSameEnteries" << termcolor::reset<< std::endl;
    mutex->unlock();

    for (int i = 0; i < playersAndTheirBets_v.size(); ++i) {
        if(playersAndTheirBets_v[i].first == nick)
        {

            auto nickFromUser = capitalismStorage->getUserFromNick(playersAndTheirBets_v[0].first);
            if(nickFromUser){
                nickFromUser->setMoney( playersAndTheirBets_v[0].second ); //return money when you remove the previous bet
                playersAndTheirBets_v.erase(playersAndTheirBets_v.begin()+i);
            }else{
                 std::cout << "CapitalismKasyno::replaceSameEnteries nickFromUSer == nullptr" << "\n";
                return;
            }

        }
    }
    playersAndTheirBets_v.push_back(std::make_pair(nick, betMoney));
}

//GameLogic
std::vector<std::pair<std::string, double>> CapitalismKasyno::gameLogic(std::vector<std::pair<std::string, double>>& playersAndTheirBets_v)
{
    std::cout << termcolor::green << "playersAndTheirBets_v.size(): " << playersAndTheirBets_v.size() << termcolor::reset<< std::endl;
    if(playersAndTheirBets_v.size()<2){ //if there is only 1 player:
        capitalismStorage->getUserFromNick(playersAndTheirBets_v[0].first)->setMoney( playersAndTheirBets_v[0].second );  //return money to the only player
        std::string lineToPost = "PRIVMSG " + room + " :\x03""07" + playersAndTheirBets_v[0].first + ", za mało graczy, musi być co najmniej 2-ch, chyba zwrócę ci twoje (" + capitalismGame->round_float( playersAndTheirBets_v[0].second ) + ") coiny \r\n\ ";
        emit sendPost(QByteArray::fromStdString(lineToPost));
        kasyno->quit(); // end thread
        //make empty vector to return
        std::vector<std::pair<std::string, double>> returner;
        return returner;
    }
    std::mt19937 rng;
    rng.seed(std::random_device()());
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<std::mt19937::result_type> dist (0,playersAndTheirBets_v.size()-1);
    std::pair<std::string, double> pair;
    int index = dist(rng);
    pair.first  = playersAndTheirBets_v[index].first;
    pair.second = playersAndTheirBets_v[index].second;
    prize = (pair.second) *2;
    std::vector<std::pair<std::string, double>> victor;
    victor.push_back(pair);
    std::cout << dist(rng)  << std::endl;
    return victor;
}

void CapitalismKasyno::gameFinished()
{
    std::cout << termcolor::red << "gameFinished()" << termcolor::reset<< std::endl;

    std::vector<std::pair<std::string, double>> temp = gameLogic(playersAndTheirBets_v); //call game logic
    if(temp.size() ==0){  //if there is only one user - meaning the returned vector from gameLogic is empty:
        kasyno->quit();   //end thread
        return;
    }
    std::string lineToPost = "PRIVMSG " + room + " :\x03""07" + "wygrał: " + temp[0].first + "! Dostaje " + capitalismGame->round_float(prize) + " coinów!"  + "\r\n\ ";
    emit sendPost(QByteArray::fromStdString(lineToPost));
    capitalismStorage->getUserFromNick(temp[0].first)->setMoney(prize);
    capitalismStorage->saveCapitalistsToFile();
    kasyno->quit(); // end thread
}

void CapitalismKasyno::debugIt(std::string str)
{
    mutex->lock();
    std::cout << termcolor::red << "" << termcolor::reset<< std::endl;
    mutex->unlock();
}

CapitalismKasyno::~CapitalismKasyno() //when thread is terminated - emit signal and delete capitalismGame so this dectructor is called
{
    std::cout << termcolor::on_blue << "~CapitalismKasyno() Destructor" << termcolor::reset<< std::endl;

    std::string msg = "gra skonczona";
    std::string lineToPost = "PRIVMSG " + room + " :\x03""07" +  msg + "\r\n\ ";
    emit sendPost(QByteArray::fromStdString(lineToPost));

}
