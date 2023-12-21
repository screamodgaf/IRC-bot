#ifndef SITEDOWNLOADER_H
#define SITEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "parser.h"
#include <QUrl>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
class Youtube : public QObject
{



    Q_OBJECT
public:
    explicit Youtube(QObject *parent = 0);
    virtual ~Youtube();
static bool youtubeActive_is;

public slots:
    void makeRequest(QString &imageUrl);
    void downloadedData()   ;
signals:
    void downloaded();
    void sendYoutubeInfo(QByteArray);
private slots:

    void fileDownloaded(QNetworkReply* pReply);
private:
    QNetworkAccessManager* m_manager;
    QByteArray m_DownloadedData;

private:
    std::string room;
//    QNetworkCookieJar jar;
//    QNetworkRequest request;

public:
    QString checkKeyword(QString data);
    void limitLengthOfYoutubeKeywords(std::string& text, size_t limit);

    void postToIRC(std::string newsText) ;




};




#endif // SITEDOWNLOADER_H
