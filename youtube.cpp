#include "youtube.h"
#include "termcolor.hpp"
#include "extract_nickandroom.h"


Youtube::Youtube( QObject *parent) :
    QObject(parent)
{
    std::cout<< "Youtube::Youtube";

    //set cookie for "zanim przejdziesz do youtube"
    m_manager = new QNetworkAccessManager( );
    //    QNetworkCookie cookie1;
    //    cookie1.setName("CONSENT=YES+42");
    //    jar.insertCookie(cookie1);
    //    request.setRawHeader("COOKIE" , "CONSENT=YES+664" );
    //       m_manager.setCookieJar(&jar);




    //    request.setRawHeader("COOKIE" , "CONSENT=YES+42" ); //works

    connect( m_manager, SIGNAL (finished(QNetworkReply*)),
             this, SLOT (fileDownloaded(QNetworkReply*))
             );

    connect(  this, SIGNAL(downloaded()),
              this, SLOT (downloadedData() )
              );
}


void Youtube::makeRequest(QString&  imageUrl)
{
    //    QNetworkAccessManager* m_manager = new QNetworkAccessManager;
    QNetworkRequest request;
    request.setRawHeader("COOKIE" , "CONSENT=YES+664" );

    std::cout<< "MAKE REQUEST\n";

    youtubeActive_is = true;
    //    imageUrl = QUrl("https://www.youtube.com/watch?v=ajTTZ7iy6Ls"); //hmm
    std::cout<< "imageUrl: " << imageUrl.toStdString() << "\n";

    qDebug() << imageUrl;
    if(imageUrl.indexOf("\r\n")!= -1)
    {
        imageUrl.replace("\r\n", "");
    }
    qDebug() << imageUrl;

    ///end of shorts

    ///========
    std::string s_index = imageUrl.toStdString();
    size_t beg = 0;
    size_t beg2 = 0;
    if((s_index.find("https://"))==std::string::npos && s_index.find("http://")==std::string::npos)
        s_index = "https://" +  s_index;
    if((beg2 = s_index.find("https://"))==std::string::npos && beg2 >0 && s_index.find("http://")==std::string::npos)
        s_index = "https://" +  s_index;

    if((beg=s_index.find("\">http"))!=std::string::npos)
        s_index = s_index.substr(0, beg-1);

    if((s_index.find("https://"))==std::string::npos && s_index.find("http://")==std::string::npos)
        s_index = "https://" +  s_index;

    size_t hbeg =0;
    if((hbeg = s_index.find("http://"))!= std::string::npos)
        s_index.replace(hbeg, 7  , "https://");



    QString QString_urlForCookies = (QString::fromStdString(s_index));
    ///alter shorts:
    if(QString_urlForCookies.indexOf("/watch?v=")==-1)
    {
        size_t beg = 0;
        if( (beg = QString_urlForCookies.indexOf(".be/"))!=-1)
        {
            QString_urlForCookies.replace(".be/", "be.com/shorts/");
        }
    }
    ///end of alter shorts:
    qDebug() << "QString_urlForCookies: " << QString_urlForCookies << "\n";

    QUrl urlForCookies (QString_urlForCookies);


    request.setUrl(urlForCookies);

    //    request.setRawHeader("COOKIE" , "CONSENT=YES+42" );
    m_manager->get(request);
    qDebug() << "PAST REQUEST";

}




void Youtube::fileDownloaded(QNetworkReply* pReply) {
    if(pReply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << pReply->errorString();
    }
    else
    {
        m_DownloadedData = pReply->readAll();
        std::cout << "fileDownloaded\n";
        //emit a signal
        pReply->deleteLater();
        emit downloaded();
    }

    //    QNetworkCookieJar jar;
    m_manager->setCookieJar(nullptr); //this solves the problem of not sending cookie for 2nd and next request !!!
}

void Youtube::downloadedData()   {
    std::cout << "Youtube::downloadedData()\n"; //repeats

    std::string website = m_DownloadedData.toStdString();

    if(website.size()!=0)
    {
        Parser parser;
        std::string title = parser.extract_tagContent(website, "title");
        //        clearWeirdSigns(title);
        std::cout << termcolor::red << "3 "<< title << termcolor::reset<< std::endl;

        //        title = "\x02\x03""04" +title + "\x02";
        title = "\u00034" +title;

        std::string begTag = "<meta name=\"keywords\" content=\"";
        std::string endTag = "\">";

        std::string keywords = "\x03""12 | \x03""13 Tags: \u000312";
        keywords +=  parser.findYoutubeKeywords(begTag, endTag, website);
        if(keywords.find("film, udostępnianie, telefon z aparatem, telefon z kamerą, bezpłatnie")!=std::string::npos)
            keywords = " \x03""12 | \u00039Nie ma tagów";
        if(keywords != " \x03""12 | \u000399Nie ma tagów")
            //            limitLengthOfYoutubeKeywords(keywords,130);

            std::cout << termcolor::red << title << termcolor::reset<< std::endl;

        title+= keywords;

        std::cout << termcolor::on_cyan << "title: " << title << termcolor::reset<< std::endl;
        if(title.size()>2000) return;
        postToIRC(title); //post to IRC
    }else
        std::cout << termcolor::on_cyan << "website empty" << termcolor::reset<< std::endl;

    // m_manager->deleteLater();
}


QString Youtube::checkKeyword(QString data)
{
    std::cout << termcolor::yellow<< "Youtube::checkKeyword" << termcolor::reset<< std::endl;

    QString indexCore;
    //std::cout << termcolor::yellow << "Youtube::provideData: " << this->thread()->currentThreadId() << termcolor::reset<< std::endl;

    if(data.indexOf("www.youtu")!=-1)
    {
        std::cout << "'www.youtu' Found" << "\n";
        Extract_NickAndRoom extract;
        room = extract.extractRoom(data);

        int indexBegin = data.indexOf("www.youtube"); //6
        //int indexEnd = data.indexOf(" "); //cant be  "\r\n" cause i dont want the end of the line but end of youtube link

        int indexEnd;
        if(data.indexOf(" ")!=-1)
            indexEnd = data.indexOf(" ");
        else
            indexEnd = data.indexOf("\r\n");

        indexCore = data.mid(indexBegin,(indexEnd -(indexBegin) ));
        std::cout<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<indexCore.toStdString() << "\n";
        return indexCore ;

    }
    else if(data.indexOf("youtu.be")!=-1)
    {

        Extract_NickAndRoom extract;
        room = extract.extractRoom(data);

        int indexEnd;
        int indexBegin = data.indexOf("youtu.be"); //6
        if(data.indexOf(" ")!=-1)
            indexEnd = data.indexOf(" ");
        else
            indexEnd = data.indexOf("\r\n");

        indexCore = data.mid(indexBegin,(indexEnd -(indexBegin) ));
        std::cout<< "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<indexCore.toStdString() << "\n";

        return indexCore ;
    }
    else
    {
        return "noLink";
    }
}

void Youtube::limitLengthOfYoutubeKeywords(std::string& text, size_t limit)
{
    Parser p;
    text = p.limitKeysLength(text, limit);

}

void Youtube::postToIRC(std::string newsText)
{


    std::cout << termcolor::yellow << "postToIRC" << termcolor::reset<< std::endl;
    std::string lineToPost;
    lineToPost = "PRIVMSG " + room + " :" + newsText + "\r\n ";
    emit sendYoutubeInfo(QByteArray::fromStdString(lineToPost));

    m_manager->clearAccessCache();
    m_manager->clearConnectionCache();
    //    m_manager->deleteResource(request);
    //    qDebug()<< "rawHeaderList: " << request.rawHeaderList();

}

Youtube::~Youtube()
{
    std::cout << "~Youtube() destructor" << "\n";

}
