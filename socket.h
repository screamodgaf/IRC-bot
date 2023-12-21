#ifndef SOCKET_H
#define SOCKET_H
#include <QAbstractSocket>
#include <QTcpSocket>
#include <chrono>
#include <QNetworkAccessManager>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkProxy>
#include <cstring>
#include <string>
#include <QEventLoop>
#include <QThread>
#include <QByteArray>
#include <QTimer>
#include <QObject>
#include <QMutex>
#include "weather.h"
#include "chatbotgtp.h"
#include "weatherforecast.h"
#include "loadlogs.h"
#include "speech.h"
#include "consoleinput.h"
#include "news1.h"
#include "talking.h"
#include "recordtalks.h"
#include "botspeech.h"
#include "controlirc.h"
#include "englishposts.h"
#include "plugin.h"
#include "greeter.h"
#include "inviteonpart.h"
#include "capitalismgame.h"
#include "Capitalism/capitalismstorage.h"
#include "ignorednicks.h"
#include "youtube.h"
#include "weatherapiforecast.h"
typedef std::chrono::time_point<std::chrono::high_resolution_clock> chronoType;

class Socket : public QObject
{
    Q_OBJECT

public:
    explicit Socket(QString IP, QString proxyPort_, QString nick, QObject *parent = nullptr);
    void ping(QString& data);


    void weather(QString &data);
    void chatbotGTP(QString &data);
    void weatherForecast(QString &data); //new
    void weatherApiForecast(QString &data);
    void speechCheck(QString data);
    void news1Refer(QString& data);
    void talkDetect(QString& data);
    void speechTrigger(QString& data);
    void ping2(QString& data);
    void answerVersionRequest(QString data);

    void control_irc(QString &data, std::string &nick);
    void greeterPaster(QString& data, int &indexLines);
    void invitePaster(QString& data);
    void youtubePaster(QString data);

    void capitalismGameDetect(QString& data);

    void englishPostsReferOnRequest(QString& data);
    void changeLineCounterOnIRC(int change);
    void checkIfServerDisconnected(QString& data);

signals:
    void signalConnected();
    void enterRoomSignal();
    void lineForYoutube(QString data);
    void proceedReadyReadSignal(QString& data);

public slots:

    void runCon();
    void connected();
    void disconnected();
    void readyReady();
    void bytesWritten(qint64);
    void proceedReadyRead(QString &data);
    void proxyHttp();
    void proxySocks();
    void joinRoom();
    void reconnect();
    void poster(const QByteArray send);
    void saveLogNow();
    void sayNowSlot();
    void updateDatabaseSLOT(std::string x);
    void englishPostsRefer();
    void pluginTrigger(QString& data);
    void antiFloodSlot();
    void greeterOf_forDelay();
    void displayError(QAbstractSocket::SocketError e);
    void stateChanged(QAbstractSocket::SocketState s);

protected:
    QTcpSocket *mSocket;
private:
    void botTalking(QString& data, std::vector<std::string> &database);
    void floodCheck(QString& data);

private:
    QString data;
    QString address;
    qint16 proxyPort;
    qint16 ircPort;
    std::string room;
    std::string room_;
    std::string nickFinal;
    std::string version;
    QString Port_final;
    QString Ip_final;

    int numberAttempts;
    QString testPrxyAdrs;
    qint16 testPrxyPort;
    QTimer *timer;
    QTimer *keepAliveTCPtimer;
    QTimer* timerFlood;
    bool roomEntered;
    std::vector<std::string> database;
    std::vector<std::string> engDatabase;
    //bool weather_is;

    Speech *speech;
    Weather wth;
    ChatbotGTP* chatbot = nullptr;
    WeatherForecast wthForecast;
    WeatherApiForecast wthApiForecast;
    ConsoleInput *consoleInput;
    News1* news1;
    Talking *talking;
    RecordTalks *recordTalks;
    BotSpeech *botSpeech;
    int indexLines; // counter of lines spoken on chat
    QString data2;
    ControlIRC *irc;
    QTimer *timerEnglish;
    QTimer *timerDelaySpeech;
    EnglishPosts englishPosts;
    int indexEnglish;
    LoadLogs *loadLogs;
    Plugin plugin;
    class AntiFlood* antiflood;
    int englishTimerFrequency;
    Plugin *p;
    Greeter greeter;
    InviteOnPart* inviteOnPart;
    CapitalismGame* capitalismGame;
    CapitalismStorage* capitalismStorage;
    IgnoredNicks ignoredNicks;
    QMutex* mutex;
    Youtube* youtube;

    chronoType start, end;
    std::chrono::duration<float> duration;

};

#endif // SOCKET_H







