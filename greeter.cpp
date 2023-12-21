#include "greeter.h"
#include "extract_nickandroom.h"
#include <QTimer>
#include <QThread>
#include <vector>
#include "loadlogs.h"
// greets on join and invites on part:

Greeter::Greeter(QObject *parent) : QObject(parent)
{

}


void Greeter::run(const QString data_, QTcpSocket *mSocket_, int &indexLines_, std::string myNick_,
                  LoadLogs* loadlogs)
{
     std::cout << termcolor::yellow<< "Greeter::run" << termcolor::reset<< std::endl;
    indexLines = &indexLines_;
    myNick = myNick_;

    data = data_;
    mSocket = mSocket_;
    greetings_v = &loadlogs->getGreetingVector();
    std::cout << "greetings_v.size() " << greetings_v->size() << std::endl;
    checkJoinPresence();
}



void Greeter::checkJoinPresence()
{

    std::cout << "Greeter::checkJoinPresence()" << std::endl;

    Extract_NickAndRoom e;
    nick = e.extractNick(data);
    room = e.extractRoomFromJoin(data);


    if(nick.size()==0 || room.size() == 0 || nick == myNick){
        return;
    }
    else
    {
        std::cout << "nick: " << nick << std::endl;
        std::cout << "room: " << room << std::endl;

//        (*indexLines)--; //so line count will go one back //TODO
        std::cout << "indexLines in Greeter: " << indexLines << " and its value: " << *indexLines << std::endl;
        pickGreetingLine();
    }

}

void Greeter::pickGreetingLine()
{
    std::cout << "pickGreetingLine()" << std::endl;

    if(Indexgreetings_v == greetings_v->size())
        Indexgreetings_v = 0;         //when index reaches end, move to vector begining

    line = (*greetings_v)[Indexgreetings_v++];

    size_t pos;
    if((pos=line.find("$nick"))!=std::string::npos) //replac $nick with current nick
        line.replace(pos, 5, nick);
    else
        line += ' ' + nick;


    //QThread::msleep(1000);
    //line  = "siemak " + nick;

    greetingsSent_is = true; //so bot doesnt greet more often than post certain amount of time

    QTimer::singleShot(5000 , this, SLOT(doPosting()));


    //doPosting();
}

void Greeter::doPosting()
{

    std::cout << "Greeter::doPosting()" << std::endl;
    std::cout << "room: " << room << "|\n";
    std::cout << "line: " << line << "|\n";
    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
    std::string lineToPost =  "PRIVMSG " + room + " :" + line + "\r\n ";



    std::cout << "Greeter:: lineToPost " << lineToPost << "|\n";

    QByteArray temp = QByteArray::fromStdString(lineToPost);
    emit postSignal(temp);


}






