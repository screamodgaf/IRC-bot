#include "englishposts.h"
#include <iostream>
#include <fstream>
#include "settings.h"
EnglishPosts::EnglishPosts(QObject *parent) : QObject(parent)
{

}

EnglishPosts::~EnglishPosts()
{
    std::cout << "~EnglishPosts() DESTRUCTOR" << "\n";

}

void EnglishPosts::checkKeyword(QString data_, QTimer* timerEnglish, int englishTimerFrequency)
{
    std::cout << "EnglishPosts::checkKeyword" << std::endl;
    data = data_;

    Extract_NickAndRoom e;
    room = e.extractRoom(data);
    if(data.indexOf("!eng")!=-1 || data.indexOf("!english")!=-1 )
    {
        timerEnglish->start(englishTimerFrequency);
        pickEnglishLine();

    }
    else
        return;
}

void EnglishPosts::fillNullPointer(LoadLogs* loadLogs_, QMutex *m)
{
    mutex = m;
    loadLogs = loadLogs_;
}

void EnglishPosts::pickEnglishLine()
{
    size_t sizeEngDatabase = loadLogs->englishDatabaseGetter().size();
    std::cout << "[[[[[[[ sizeEngDatabase: " << sizeEngDatabase << "\n";
    srand(time(nullptr));
    int r = std::rand()%(sizeEngDatabase-1);
    englishLine = loadLogs->englishDatabaseGetter()[r];


    doPostingOnRequest(englishLine);
}

void EnglishPosts::doPostingOnRequest(std::string& englishLine_)
{
    englishLine_.erase(std::remove(englishLine_.begin(), englishLine_.end(), '\r'), englishLine_.end());
    std::string lineToPost =  "PRIVMSG " + room +" :"  + englishLine_ + "\r\n ";
    std::cout << "void EnglishPosts::doPostingOnRequest: " << lineToPost << " |||||||||||||||||||||| " << "\n";
    QByteArray temp = QByteArray::fromStdString(lineToPost);
    emit postSignal(temp);

}




void EnglishPosts::doPosting(int index )
{
    std::cout << termcolor::yellow << "EnglishPosts::doPosting" << termcolor::reset<< std::endl;

    std::string x = loadLogs->englishDatabaseGetter()[index];

    std::cout << "x: " << x << "\n";
    x.erase(std::remove(x.begin(), x.end(), '\r'), x.end());


    std::string lineToPost =  "PRIVMSG "+ Settings::getMyRoom().toStdString() + " :" +  x  + "\r\n ";
    QByteArray temp = QByteArray::fromStdString(lineToPost);
    emit postSignal(temp);
}



