#include "inviteonpart.h"
#include "extract_nickandroom.h"
#include <QTimer>
#include <QThread>
#include <vector>
#include "loadlogs.h"
#include "settings.h"
// invites on part:
InviteOnPart::InviteOnPart(QObject *parent) : QObject(parent)
{

}

void InviteOnPart::run(const QString data_, QTcpSocket *mSocket_, std::string myNick_,
                  LoadLogs* loadlogs)
{
    std::cout << termcolor::yellow<< "InviteOnPart::run" << termcolor::reset<< std::endl;

    myNick = myNick_;

    data = data_;
    mSocket = mSocket_;
    //greetings_v = &loadlogs->getGreetingVector();
    //std::cout << "greetings_v.size() " << greetings_v->size() << std::endl;
    checkPartPresence();
}



void InviteOnPart::checkPartPresence()
{

    std::cout << "InviteOnPart::  checkPartPresence()" << std::endl;

    Extract_NickAndRoom e;
    nick = e.extractNick(data);
    room = e.extractRoomFromDeparture(data);


    if(nick.size()==0 || room.size() == 0 || nick == myNick){
        return;
    }
    else{
         std::cout << "nick: " << nick << std::endl;
         std::cout << "room: " << room << std::endl;
         QTimer::singleShot(5000 , this, SLOT(postInvite()));
    }
}


void InviteOnPart::postInvite()
{
    std::cout << "INVITING INVITING \n\n\n\n" << std::endl;

    std::string InviteToPost =  "INVITE " + nick + " "+ Settings::getMyRoom().toStdString()  + line + "\r\n ";

    const char *send = InviteToPost.c_str();

    std::cout << "Greeter:: sending invite" << std::endl;
    mSocket->write(send);
    mSocket->flush();
    InviteToPost.clear(); //clear before next post
    mSocket->flush();
    mSocket->reset();
}
