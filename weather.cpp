#include "weather.h"
#include <QThread>
#include "extract_nickandroom.h"
#include <QDir>
#include <QCoreApplication>
Weather::Weather(QObject *parent) : QObject(parent)
{

}

Weather::~Weather()
{
    std::cout << "WEATHER DESTRUCTOR" << "\n";
}

void Weather::weatherKeyWord(QString data)
{
    std::cout << "Weather::weatherKeyWord" << std::endl;
    if(data.indexOf("!pogoda ")!=-1)
    {
        std::cout << "lineeeeeee: " << data.toStdString() << "\n";
        Extract_NickAndRoom extract;
        room = extract.extractRoom(data);
        std::cout<< "room=======: <<room;\n";


    int indexBegin = data.indexOf("!pogoda "); //6
    int indexEnd = data.indexOf("\r\n");

    QString indexCore = data.mid(indexBegin+8,(indexEnd -(indexBegin+8) ));
    std::cout<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<indexCore.toStdString() << "n";

    process(indexCore);

    }
    else
    {
        return;
    }
}

void Weather::process(QString indexCore)
{
    QDir directory = QCoreApplication::applicationDirPath();
    QString relativePath = directory.absoluteFilePath("config/weather.py");

    std::cout << "relativePath: " << relativePath.toStdString() << "\n";
//    QString cmdCommand = "python3";
     QString cmdCommand = "python3";

    QStringList list;
    list << relativePath << indexCore;

    QProcess *qprocess= new QProcess(this);
    qprocess->setProgram(cmdCommand);
    qprocess->setArguments(list);
    qprocess->start();
    qprocess->waitForFinished(15000);
qDebug() << "weasther list: " << list;


    while(qprocess->canReadLine())
    {
        QByteArray tempByteArr = qprocess->readLine();
        std::string temp = tempByteArr.toStdString();
        doPosting(temp);
        //std::cout<< "temp: " << temp << "\n";
        QThread::msleep(250);

    }

    qprocess->close();

    qprocess = nullptr;
    delete qprocess;
}


void Weather::doPosting(std::string string)
{

     std::string lineToPost;
     lineToPost = "PRIVMSG " + room + " :" + string + "\r\n\ ";
     QByteArray temp = QByteArray::fromStdString(lineToPost);
    emit postSignal(temp);

 }
