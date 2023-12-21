#include "chatbotgtp.h"
#include <QThread>
#include "extract_nickandroom.h"
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include "settings.h"
ChatbotGTP::ChatbotGTP(QObject *parent) : QObject(parent)
{
    qDebug() << "ChatbotGTP::ChatbotGTP Constructor";

}

ChatbotGTP::~ChatbotGTP()
{
    qDebug() << "ChatbotGTP DESTRUCTOR";

}


bool ChatbotGTP::checIgnored(const QString &data)
{
    std::vector<QString> ignoredNicks_v = {"Koari", "Nugget" };
    for (int i = 0; i < ignoredNicks_v.size(); ++i) {
        if(data.indexOf(ignoredNicks_v[i])  != -1){
            return false;
        }
    }
    return true;
}

void ChatbotGTP::chatbotKeyWord(QString data)
{
    qDebug() << "ChatbotGTP::chatbotKeyWord"  ;

    if(data.indexOf("#")==-1) return; //no room
    //if(data.indexOf("irc")!=-1) return;
    if(data.indexOf("PRIVMSG")==-1) return;
    if(checIgnored(data) == false) return;

    if(data.indexOf(Settings::getMyNick())!=-1)   //turn off bot
    {
        std::cout << "lineeeeeee ChatbotGTP: " << data.toStdString() << "\n";
        Extract_NickAndRoom extract;
        room = extract.extractRoom(data);
        nick = extract.extractNick(data);



        QString indexCore = QString::fromStdString(extract.extractText(data));

        if(indexCore.indexOf(Settings::getMyNick() + ":")!=-1 )
            indexCore = indexCore.remove(Settings::getMyNick() + ":");

        if(indexCore.size()<10) return;
//        qDebug() << "@@@@@@@@@@@@@@@@@@@@@indexCore@@@@@@@@@@@@@@@@@@@@@@@: " + indexCore  ;


        //CONTEXT:
//        if(lastMessage.size()>3){
//            QString askersNick = "Ja mam na imię " +  QString::fromStdString( nick);
//            indexCore = lastMessage + ". " + askersNick + ". " + indexCore;
//        }
//        else
//        {
//            lastMessage = indexCore;
//        }





qDebug() << "@@@@@@@@@@@@@@@@@@@@@indexCore@@@@@@@@@@@@@@@@@@@@@@@: " + indexCore  ;
        process(indexCore);
    }
    else
    {
        return;
    }
}

void ChatbotGTP::process(QString indexCore)
{
    QDir directory = QCoreApplication::applicationDirPath();
    QString relativePath = directory.absoluteFilePath("config/chatbot.py");

    qDebug() << "ChatbotGTP relativePath: " << relativePath  ;
    //    QString cmdCommand = "python3";
    QString cmdCommand = "python3";

    QStringList list;
    //    indexCore = "pomocy";
    list << relativePath << indexCore;
    qDebug() << "ChatbotGTP indexCore: " << indexCore  ;
    QProcess *qprocess= new QProcess(this);
    qprocess->readAllStandardOutput();

    qprocess->setProgram(cmdCommand);
    qprocess->setArguments(list);
    qDebug() << "listgte: " << list;
    qprocess->start();

    qprocess->waitForFinished(45000);


    std::string temp;
    while(qprocess->canReadLine())
    {
        QByteArray tempByteArr = qprocess->readLine();
        qDebug() << "chatbot2" << tempByteArr;
        std::string temp = tempByteArr.toStdString();

        if(temp.size()>200){
            auto vec = separateLine(temp);
            for (int i = 0; i < vec.size(); ++i) {
                doPosting(vec[i]);
                QThread::msleep(250);
            };
        }else
            doPosting(temp);


        QThread::msleep(250);

    }
    qprocess->waitForBytesWritten();
    qprocess->close();

    qprocess->deleteLater();
//    qprocess = nullptr;

//    delete qprocess;
}

std::vector<std::string> ChatbotGTP::separateLine(std::string string){


    std::vector<std::string> separatedLines_v;
    size_t divisionChunk = 200;
    size_t divJump = string.size()/ divisionChunk;
    size_t start = 0;

    for (int i = 0; i < divJump; i++) {
        start = i* divisionChunk;
        separatedLines_v.push_back(string.substr(start,  divisionChunk));
    }

    std::cout << "start+divisionChunk: " << start+divisionChunk << "\n";
    std::cout << "string.size(): " << string.size() << "\n";
    std::cout << "start: " << start << "\n";
     std::cout << "string.size()-(start+divisionChunk): " << string.size()-(start+divisionChunk) << "\n";
    std::string lastString = string.substr(start+divisionChunk,  string.size()-(start+divisionChunk));

    separatedLines_v.push_back(lastString);
    return separatedLines_v;
}



void ChatbotGTP::doPosting(std::string string)
{
    if(string.size()<3) return;
    qDebug() << "ChatbotGTP::doPosting(std::string string)";
    std::string lineToPost;
    qDebug() << "chatbot2" << QString::fromStdString( string);

    lineToPost = "PRIVMSG " + room + " :" + string + "\r\n\ ";


    //    lineToPost = insertNewLines(lineToPost);


    QByteArray temp = QByteArray::fromStdString(lineToPost);
    qDebug() << "getsignal: " << temp;
    emit postSignal(temp);

}



