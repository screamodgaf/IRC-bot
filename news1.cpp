#include "news1.h"
#include <QByteArray>
#include "extract_nickandroom.h"
News1::News1(QObject *parent): QObject(parent)
{

    manager = new QNetworkAccessManager(this);

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(readyRead(QNetworkReply*)));
}

void News1::makeRequest()
{

qDebug() << "News1::makeRequest()";

    //request.setSslConfiguration(QSslConfiguration::defaultConfiguration().);
    std::cout << "News1::makeRequest()\n";
    request.setUrl(QUrl("https://www.defence24.pl/a"));
    QString text = "User-Agent" ;
    QString text2 =  "Mozilla/5.0 (Symbian; U; N8-00; fi-FI) AppleWebKit/534.3 (KHTML, like Gecko) Qt/4.7.4" ;
    request.setRawHeader(QByteArray(text.toLocal8Bit() ) ,
                         QByteArray(text2.toLocal8Bit()));
    manager->get(request);

}


void News1::readyRead(QNetworkReply *replay)
{
    qDebug() << replay->readAll();
    QByteArray dataTemp = replay->readAll();
    website = dataTemp.toStdString();
    qDebug() << " News1::readyRead: " << dataTemp;

    if(website.size()!=0)
    {
        Extract_NickAndRoom extractor;

        room = extractor.extractRoom(data);

        Parser *parser = new Parser;
        //std::cout << parser.extract_link(text) << "\n";
        for (int var = 1; var <= 10; ++var)
        {
            std::string newsText = parser->goIntoTag(website, "title",var);

            //std::string newText2 = parser.extract_tagContent(newText, "a ");
            std::cout << newsText    << "\n";
            //std::string newText3 = parser.extract_link(newText);
            //std::cout << newText3  << "\n";

            postToIRC(clear(newsText) ); //post to IRC
            QThread::sleep(3);
        }
    }
}

void News1::checkKeyword(QString data_)
{
    qDebug() << "News1::checkKeyword(QString data_)";
    data=data_;

    if(data.indexOf("!news1")!=-1 || data.indexOf("daj newsa 1")!=-1 )
    {
        makeRequest();
    }
    else
        return;
}

void News1::postToIRC(std::string newsText)
{
    std::string lineToPost;
    lineToPost = "PRIVMSG " + room + " :" +  newsText + "\r\n";
    std::cout << "news lineToPost: " +lineToPost << "\n";
    emit mySignal(QByteArray::fromStdString(lineToPost));

}

std::string News1::clear(std::string& text)
{
    size_t prefix =0;
    size_t surfix =0;
    std::string core;
    if(((prefix=text.find("<![CDATA["))!=std::string::npos) &&
            ((surfix= text.find("]]", prefix))!=std::string::npos))
    {
       core = text.substr(prefix+9, surfix-(prefix+9));
       std::cout <<core  ;
    }
    return core;
}
