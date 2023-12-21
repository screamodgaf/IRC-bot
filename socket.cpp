#include "socket.h"
#include <QThread>
#include <time.h>
#include <QString>
#include <iostream>
#include "colourmanipulation.h"
#include "settings.h"
#include "antiflood.h"

bool Youtube::youtubeActive_is = false;
//bool Youtube::cookieSent = true;
//int AntiFlood::floodlines =0; //static initialization of floodlines
int CapitalismGame::earningCounter = 0; //initialize static

unsigned int  rand4GreetingIndex = (srand(time(0)), rand()%35);  //so greeting index starts with random value at program start

Socket::Socket(QString IP, QString proxyPort_,
               QString nick, QObject *parent) : QObject(parent)
{
    nickFinal = nick.toStdString();
    Ip_final = IP;
    proxyPort = proxyPort_.toUShort();



    ///SERVER:
    address = Settings::getMyServer();


    ircPort = Settings::getMyPort().toUShort();   // Porty: 6665 - 6668  and SSL (QSslSocket): 6697, 6601, 7002

    qDebug()<< "SOCKET CLASS";
    indexLines = 0; //related to bot talk answers

    //socket
    mSocket = new QTcpSocket;
    mSocket->setSocketOption(QAbstractSocket::KeepAliveOption, true);


    // load logs
    loadLogs = new LoadLogs;
    loadLogs->load();
    database = loadLogs->databaseGetter();
    loadLogs->loadEnglishLogs();
    loadLogs->loadtexts();
    loadLogs->loadGreetings();
    loadLogs->loadListOfCapitalists();

    // filling null pointer in EnglishPosts
    englishPosts.fillNullPointer(loadLogs, mutex);

    //set random value for greeting index so when program starts greeting line is random and then moves by ++in vector:

    //antiflood:
    antiflood = new AntiFlood;

    std::cout << "rand4GreetingIndex: " << rand4GreetingIndex << std::endl;
    //timer for news2
    QTimer *timer = new QTimer(this);
    timer->start(60000);

    //timer for english posts:
    timerEnglish = new QTimer(this);
    englishTimerFrequency = 9999999 ; //1200000;
    timerEnglish->start(englishTimerFrequency);


    keepAliveTCPtimer = new QTimer(this);
    keepAliveTCPtimer->start(60000);
    start = std::chrono::high_resolution_clock::now();

    chatbot = new ChatbotGTP(this);

    //timer for antiflood
    //    timerFlood = new QTimer;
    //    timerFlood->start(2000);
    //    connect(timerFlood, SIGNAL(timeout()), this, SLOT(antiFloodSlot())); //timers

    //speach:
    speech = new Speech(*loadLogs);
    speech->getKeywordsAndText();

    //delay speech timer:
    timerDelaySpeech = new QTimer;

    //console input:
    QThread *thread = new QThread;
    consoleInput = new ConsoleInput();
    consoleInput->startConsole(thread);
    consoleInput->moveToThread(thread);
    thread->start();

    //capitalism storage
    capitalismStorage = new CapitalismStorage(this, loadLogs, capitalismGame);
    capitalismStorage->attachAssetsToUser(); //get
    capitalismStorage->determineLevels();

    //void ConsoleInput::startConsole(QThread *thread_, QTcpSocket *mSocket_)
    //news


    //record talking" on a room in separate thread:
    recordTalks = new RecordTalks(this, capitalismStorage);



    //bot speech in separate thread:
    botSpeech = new BotSpeech(this);


    //bot english posts
    indexEnglish =0;
    p = new Plugin;
    //irc control
    irc = new ControlIRC;


    //inviteOnPart:
    inviteOnPart = new InviteOnPart(this);
    //ignored nicks:
    ignoredNicks.fill_ignoredNicks_v();

    capitalismGame = new CapitalismGame(this, antiflood, mutex);

    //     Youtube
    //    QThread *thread9 = new QThread;
    youtube = new Youtube(this);
    //    youtube->moveToThread(thread9);
    //    connect(this, SIGNAL(lineForYoutube(QString)), youtube, SLOT(provideData(QString)));
    //    thread9->start();
    //    youtube->startYoutube(thread9, mSocket);

    news1 = new News1(this);


    //proxyHttp();  //uncomment to activate proxy (after proxy is set in settings.cpp)
    connect(mSocket, SIGNAL(connected()),this, SLOT(connected()));
    connect(mSocket, SIGNAL(readyRead()), this, SLOT(readyReady()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(reconnect()));
    connect(mSocket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(mSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(mSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));


    connect(this, SIGNAL(enterRoomSignal()),
            this, SLOT(joinRoom()));


    connect(capitalismGame, SIGNAL(postSignal(QByteArray)),
            this, SLOT(poster(QByteArray)));

    connect(&greeter, SIGNAL(postSignal(QByteArray)),
            this, SLOT(poster(QByteArray)));

    connect(&englishPosts, SIGNAL(postSignal(QByteArray)),
            this, SLOT(poster(QByteArray)));

    connect(&wth, SIGNAL(postSignal(QByteArray)),
            this, SLOT(poster(QByteArray)));

    connect(chatbot, SIGNAL(postSignal(QByteArray)),
            this, SLOT(poster(QByteArray)));


    connect(youtube, &Youtube::sendYoutubeInfo, this, &Socket::poster);


    connect(consoleInput, SIGNAL(mySignal(QByteArray)), //for console input
            this , SLOT( poster(QByteArray)));

    connect(news1, SIGNAL(mySignal(QByteArray)), //for news1
            this , SLOT( poster(QByteArray)));

    connect(botSpeech, SIGNAL(postSignal(QByteArray)),
            this, SLOT(poster(QByteArray)));

    connect(consoleInput, SIGNAL(saveSignal()),
            this, SLOT(saveLogNow()));

    connect(consoleInput, &ConsoleInput::sayNow,
            this, &Socket::sayNowSlot);


    //    connect(greeter, &Greeter::greetedSuccessfully ,  //for turn off timer so bot wont greet too many times within given time
    //      this, &Socket::sayNowSlot);

    connect(recordTalks, &RecordTalks::textUpdateSignal,
            this, &Socket::updateDatabaseSLOT);


    connect(timerEnglish, SIGNAL(timeout()),
            this, SLOT(englishPostsRefer()));
    //connect(antiflood, SIGNAL(floodingUser(std::vector<std::string>)),
    //      antiflood, SLOT(unBan(std::vector<std::string>)));

    connect(antiflood, SIGNAL(unbanInfoPost(QByteArray)),
            this, SLOT(poster(QByteArray)));

    connect(this, &Socket::proceedReadyReadSignal, this, &Socket::proceedReadyRead);

    runCon();
}


void Socket::runCon()
{

    std::cout << termcolor::blue << "runCon(): nick: " << nickFinal << termcolor::reset<< std::endl;

    roomEntered=false;
    qDebug() << "Socket::runCon()    Connecting...";

    // this is not blocking call
    //    if(mSocket->isOpen()) mSocket->close();

   //mSocket->connectToHost(address, ircPort);

    std::cout << "server ADDRESS " << address.toStdString() << "\n";
   qDebug() << "server PORT " << ircPort << "\n";
  mSocket->connectToHost(address, ircPort);




    start = std::chrono::high_resolution_clock::now(); //reset time
    // we need to wait...
    if(!mSocket->waitForConnected(10000))
    {
        start = std::chrono::high_resolution_clock::now(); //reset time
        qDebug() << "Error: " << mSocket->errorString();
        QTimer::singleShot(5000, this, SLOT(reconnect())); //RECONNECTION
    }


}


void Socket::connected()
{
    qDebug() << "Connected!";
    std::cout << "Connected! nickFinal: " <<  nickFinal;




    std::string _user = "NICK " + nickFinal +   "\nUSER user 0 * :user\r\n";





    mSocket->write(_user.c_str());


    //===============================
    mSocket->flush();


    room_ = "JOIN " + room  + "\r\n";

    //mSocket->write(room_.c_str());

    //mSocket->waitForBytesWritten();




    ////mSocket->flush();
    //      mSocket->waitForBytesWritten();
    mSocket->write("PRIVMSG NickServ IDENTIFY dewfdfew\r\n\r\n\r\n");
    


}



void Socket::disconnected()
{
    mSocket->close();
    mSocket->waitForBytesWritten();
    std::cout << "Socket::disconnected()" << "\n";
}

void Socket::bytesWritten(qint64 bytes)
{
    std::cout << bytes << " bytes written...\n";
}

void Socket::readyReady()
{


    while (mSocket->canReadLine()) {
        data = mSocket->readLine();

        std::cout << termcolor::on_magenta << "START @@@@@@@@@@@@ mSocket->readLine();: " << data.toStdString() << termcolor::reset<< std::endl;

        ping(data); //check for ping while being connected
            ping2( data); //check for ctcp version while being connected
//        answerVersionRequest(data); //check for ctcp version while being connected
        std::cout << termcolor::yellow<< " 2 -------------------------- Past ping2" << termcolor::reset<< std::endl;

        emit proceedReadyReadSignal(data);
    }
}


void Socket::proceedReadyRead(QString& data)
{


    //    while(mSocket->canReadLine())
    //    {


    //        if(ignoredNicks.checkForIgnoredNicks(data)== true) //break if ignored sender's nick found
    //            //return;
    //            break;

    checkIfServerDisconnected(data);
    weather(data); //checks if word pogoda is typed on chat
    chatbotGTP(data);
    youtubePaster(data); // turn off
    weatherForecast(data); //checks if word forecast is typed on chat
    weatherApiForecast(data);
    speechCheck(data); //checks if keyword for speech is typed
    news1Refer(data); //checks if keyword for news is typed
    talkDetect(data);
    englishPostsReferOnRequest(data); //checks if keyword for english is typed
    saveLogNow();     //checks if keyword for save is typed
    //speechTrigger(data);
    botTalking(data, database);
    control_irc(data, nickFinal);
    //        floodCheck(data); // checks for flood
    pluginTrigger(data);
    greeterPaster(data, indexLines);
    //            invitePaster(data, mSocket);

    capitalismGameDetect(data);

    std::cout << termcolor::on_blue << "indexLines: " << indexLines << termcolor::reset<< std::endl;



    std::cout << termcolor::on_red << " ========== END OF ITERATION =========" << termcolor::reset<< std::endl;
    //    }
}


void Socket::proxyHttp()
{
    qDebug() << "proxyHttp()";



    QNetworkProxy proxy;
    proxy.setUser(Settings::getProxyUSer());
    proxy.setPassword(Settings::getProxyPassword());
    proxy.setType(QNetworkProxy::Socks5Proxy);
    proxy.setHostName(Ip_final);
    proxy.setPort(proxyPort );
    QNetworkProxy::setApplicationProxy(proxy); // turn off proxy
}

void Socket::proxySocks()
{

}

void Socket::joinRoom()
{
    const QString join = "JOIN " + Settings::getMyRoom() + "\r\n\r\n\r\n";
    mSocket->write(join.toUtf8());

    mSocket->flush();
    //identyfy the user with password:
    //mSocket->write("PRIVMSG NickServ IDENTIFY somepassword\r\n\r\n\r\n");
    //================



}

void Socket::reconnect()
{
    qDebug() << "RECONNECT , RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT RECONNECT  ";


    //mSocket->abort(); //hmm
    mSocket->reset();
    QTimer::singleShot(10000, this, SLOT(runCon()));
}

void Socket::poster(const QByteArray send)   //sending to irc from many classes news, console itd
{
    std::cout << termcolor::on_magenta << "Socket::poster: " << send.toStdString() << termcolor::reset<< std::endl;

    qDebug() << "Socket::posterSENT: " << send  ;

    mSocket->write(send);
    mSocket->waitForBytesWritten(5000); //without this weatrher sends only 1 line

    //    mutex->lock();

    //    std::string send_ = send.toStdString();
    //    ColourManipulation c;
    //    send_ = c.removePolSigns(send_);
    //    mSocket->write(send_.c_str());

    //    mSocket->waitForBytesWritten();


}


void Socket::ping(QString &data)
{
    std::cout << " --- in " << termcolor::on_magenta << "Socket::ping" << termcolor::reset << std::endl;
    if(data.indexOf("PING")!=-1 && data.indexOf("#")==-1 &&
            data.indexOf("<")==-1  && data.indexOf("PING")==0 /* PING must be first in data stream*/)

    {
        std::cout << " ---" << termcolor::on_magenta << "found PING and GAVE PONG" << termcolor::reset << std::endl;
        qDebug() << "TTTTTTTTTTTTTTTTTTTTTTTT " << data.indexOf("PING") << "TTTTTTTTTTTTTTTTTTTTTT";

        int index = data.indexOf("PING");
        data[index+1] = 'O';
        std::string mString = data.toStdString();
        mString+="\r\n";
        const char * y = mString.c_str();

        mSocket->write(y);
        mSocket->reset();
        if(roomEntered==false)
        {
            emit enterRoomSignal();
            roomEntered=true;
        }


        //        mSocket->waitForBytesWritten();
        qDebug() << "I WROTE: " << y;
        data.clear();
        mString.clear();

        mSocket->flush();
        //        mSocket->reset();
    }
}




void Socket::ping2(QString &data) //CTCP VERSION
{
    size_t pos1, pos2;
    if((pos1=data.toStdString().find(":"))!=std::string::npos);
    if((pos2=data.toStdString().find("!",pos1))!=std::string::npos);
    std::string nick = data.toStdString().substr(pos1+1, pos2-(pos1+1));

    if(data.indexOf("VERSION")!=-1 && data.indexOf("#")==-1 &&
            data.indexOf("<")==-1 && data.indexOf("INIT")==-1 )
    {


        int index = data.indexOf("PING");
        data[index+1] = 'O';
        //std::string mString =  data.toStdString() + " irssi v1.0.30-1~Buhaha";


        std::string mString1  = "NOTICE " + nick  + " :\x01VERSION bez wersji, Łosiu\x01\r\n";

        //mString+="\r\n\r\n\r\n";
        const char * y = mString1.c_str();
        //        mSocket->WriteOnly;
        mSocket->write(y);
        //        mSocket->reset();

        if(roomEntered==false)
        {
            emit enterRoomSignal();
            roomEntered=true;
        }


        //        mSocket->waitForBytesWritten();
        qDebug() << "I WROTE: " << y;
        data.clear();
        mString1.clear();
        qDebug() << "CTCPPPPPPPPPPPPPPPPPP: " << y;
        mSocket->flush();
        //        mSocket->reset();
    }
}

void Socket::answerVersionRequest(QString data) //CTCP VERSION
{
    size_t pos1, pos2;
    if((pos1=data.toStdString().find(":"))!=std::string::npos);
    if((pos2=data.toStdString().find("!",pos1))!=std::string::npos);
    std::string nick = data.toStdString().substr(pos1+1, pos2-(pos1+1));

    qDebug() << " CTCP nick: " << QString::fromStdString(nick);
    if(data.indexOf("VERSION")!=-1 && data.indexOf("#")==-1 &&
            data.indexOf("<")==-1 && data.indexOf("INIT")==-1 )
    {


        //        int index = data.indexOf("PING");
        //        data[index+1] = 'O';
        //std::string mString =  data.toStdString() + " irssi v1.0.30-1~Buhaha";


        //        std::string mString1  = "NOTICE " + nick  + " :\x01VERSION Windows 12, Firefox 150.02.1\x01\r\n";

        std::string mString1  = "NOTICE Global :\x01VERSION mirc\x01\n\r";

        //mString+="\r\n\r\n\r\n";
        const char * y = mString1.c_str();
        //        m_Socket->WriteOnly;
        mSocket->write(y);
        //        m_Socket->reset();

        if(roomEntered==false)
        {
            emit enterRoomSignal();
            roomEntered=true;
        }


        //        m_Socket->waitForBytesWritten();
        qDebug() << "I WROTE: " << y;
        data.clear();
        mString1.clear();
        qDebug() << "CTCPPPPPPPPPPPPPPPPPP: " << y;
        mSocket->flush();
        //        m_Socket->reset();
    }
}

bool ConsoleInput::weather_is=1;
void Socket::weather(QString &data)
{
    std::cout << termcolor::magenta << "Socket::weather" << termcolor::reset<< std::endl;

    bool weather_isOn = consoleInput->weatherState();
    std::cout << "c.weatherState(): " << weather_isOn << "\n";

    if(weather_isOn==true)
        wth.weatherKeyWord(data);
    else
        return;

}

void Socket::chatbotGTP(QString &data)
{
    qDebug() << "void Socket::chatbotGTP(QString &data)";
    chatbot->chatbotKeyWord(data);
}

void Socket::weatherForecast(QString &data)
{
    std::cout << termcolor::magenta << "Socket::weatherForecast" << termcolor::reset<< std::endl;
    wthForecast.WeatherForecastKeyWord(data, mSocket);
}

void Socket::weatherApiForecast(QString &data)
{
    std::cout << termcolor::magenta << "Socket::weatherApiForecast" << termcolor::reset<< std::endl;

    wthApiForecast.WeatherForecastKeyWord(data, mSocket);
}

void Socket::speechCheck(QString data)
{
    //each time message comes from chat this is executed:
    speech->checkKeywordPresence(data);
}


bool ConsoleInput::news_is =1;

void Socket::news1Refer(QString &data)
{
    ConsoleInput c;
    if(c.newsState()==true)
        news1->checkKeyword(data );
}



void Socket::speechTrigger(QString &data)
{
    ConsoleInput c;
    if(c.newsState()==true);
    // news4->checkKeyword(data);
}

bool ConsoleInput::recordTalk_is = 1; //here i can turn off logging by changing to 0



void Socket::talkDetect(QString &data)
{
    if(consoleInput->recordTalkState()==true)
        recordTalks->run(data);
}


bool ConsoleInput::saveNow_is = 0;
void Socket::saveLogNow()
{
    if(consoleInput->saveNowState()==true)
    {
        std::cout << "#c.saveNowState() ==true" << "\n";
        recordTalks->run();
        consoleInput->set_saveNow_is_toFalse();
    }
}

void Socket::sayNowSlot() // normal bot speech every 5 lines
{
    indexLines=5;   // to activate speak now
    botSpeech->runBotSpeech(data2, database, loadLogs ,  true);
    indexLines=0;
}

void Socket::updateDatabaseSLOT(std::string textUpdate)
{
    std::cout << "Socket::updateDatabaseSLOT - updated with: " << textUpdate << "\n";
    database.push_back(textUpdate);
}

bool ConsoleInput::botSpeech_is = 0; //turn off talking
void Socket::botTalking(QString &data, std::vector<std::string> &database)
{
    std::cout << termcolor::red << "void Socket::botTalking" << termcolor::reset<< std::endl;
    Extract_NickAndRoom e;
    if(e.extractText(data).size()>2)
        indexLines++;                   //rise numbers of lines spoken on chat

    if(e.extractText(data).size()>2 && consoleInput->botSpeechState()==true && indexLines ==5)
    {
        //        indexLines++;
        //        botSpeech->runBotSpeech(data, database, loadLogs, false );
        //TODO
        auto addingKeywordToData =  Settings::getMyNick() + " " +data ;
        chatbot->chatbotKeyWord(addingKeywordToData); ///activate bot gtp talk
        indexLines=0;
    }
}

void Socket::changeLineCounterOnIRC(int change)
{
    //    indexLines+=change;
}

void Socket::checkIfServerDisconnected(QString &data)
{
    std::cout << termcolor::yellow << "Socket::checkIfServerDisconnected(QString &data): " << termcolor::reset<< std::endl;

    std::string ircMsg = data.toStdString();
    size_t pos = 0;
    if((pos = ircMsg.find(":RECONNECTclient"))!= std::string::npos && pos ==0)
    {
        std::cout << termcolor::on_red << "Socket::checkIfServerDisconnected(QString &data): " << data.toStdString() << termcolor::reset<< std::endl;
        reconnect();
    }
}

// add to current database the new 60 lines:

void Socket::control_irc(QString &data, std::string &nick)
{

    irc->run(data, nick, mSocket, consoleInput, "nick");
}


//greetings on join:
bool ConsoleInput::greetings_is=1;    //from console
bool Greeter::greetingsSent_is = 0;    //from time turn off so bot doesnt greet too often
unsigned int Greeter::Indexgreetings_v = rand4GreetingIndex;


void Socket::greeterPaster(QString &data, int &indexLines)
{
    std::cout << "Socket::greeterPaster" << "\n";
    std::cout << "&indexLines in socket: " << &indexLines << " and its value: " << indexLines << std::endl;
    bool greetings_isOn = consoleInput->greetingsState();
    std::cout << "c.greetings_isOn(): " << greetings_isOn << "\n";
    if(greetings_isOn==true && Greeter::greetingsSent_is ==false){    //greetings allowed when greetingsSent_is ==false
        std::cout << "TRUE if(greetings_isOn==true && Greeter::greetingsSent_is ==false){ " << "\n";
        greeter.run(data, mSocket, indexLines, nickFinal, loadLogs); //puttin my current nick in, so i wont greet myself

        if(Greeter::greetingsSent_is == true){    //time passed after which another greeting is allowed
            QTimer::singleShot(10000  , this, SLOT(greeterOf_forDelay()));   //restoring greetingsSent_is ==false - so greetings is allowed, after it was set unallowed (greetingsSent_is == true) in Greeter class after successful greeting
        }
    }
    else{
        std::cout << "FALSE: if(greetings_isOn==true && Greeter::greetingsSent_is ==false){   " << "\n";
        return;
    }
}

void Socket::greeterOf_forDelay() //so there is timer delay between greetings on join
{
    Greeter::greetingsSent_is = false;
}

void Socket::displayError(QAbstractSocket::SocketError e)
{
    qDebug() << "QAbstractSocket::SocketError: " << e;
}

void Socket::stateChanged(QAbstractSocket::SocketState s)
{
    qDebug() << "stateChanged(QAbstractSocket::SocketState: " << s;
}

//invite:
void Socket::invitePaster(QString &data)
{

    bool greetings_isOn = consoleInput->greetingsState();
    if(greetings_isOn==true){       //when greetings is off - invite also off
        inviteOnPart->run(data, mSocket, nickFinal, loadLogs); //puttin my current nick in, so i wont greet myself
    }
    else
        return;
}

//Youtube:
void Socket::youtubePaster(QString data)
{

    std::cout << termcolor::yellow << "void Socket::youtubePaster" << termcolor::reset<< std::endl;

    if( Youtube::youtubeActive_is==false){

        QString temp = youtube->checkKeyword(data);
        //        qDebug() << "-temp: " <<  temp;
        //        qDebug() << "-data: " <<  data;
        //        if(temp != "noLink") //check if contains word youtube
        if(data.indexOf("youtube")!= -1 )
        {
            std::cout << termcolor::red << "temp from checkKeyword: " << temp.toStdString() << termcolor::reset<< std::endl;

            youtube->makeRequest(temp);

            //
            --indexLines; //so bot doesnt speak for youtube links, cause it may cause crash
        }

        Youtube::youtubeActive_is= false;
    }
}



bool ConsoleInput::capitalismGame_is = 1; //here i can turn off game by changing to 0
void Socket::capitalismGameDetect(QString &data)
{
    if(consoleInput->capitalismState()==true)
        capitalismGame->run(data, capitalismStorage);
    else
        return;
}



bool ConsoleInput::english_is = 1;
void Socket::englishPostsRefer()
{

    std::cout << "@@@@@@@@@ english timer" << "\n";

    if(indexEnglish >= (loadLogs->englishDatabaseGetter().size())-1)
        indexEnglish=0;

    if(consoleInput->englishPostsState()==true)
    {
        englishPosts.doPosting(indexEnglish++);
    }
}

void Socket::englishPostsReferOnRequest(QString &data)
{
    //std::cout << "@@@englishPostsReferOnRequest " << "\n";

    if(consoleInput->englishPostsState()==true)
    {
        englishPosts.checkKeyword(data, timerEnglish, englishTimerFrequency);
        //p->mi1();// activates one single shoot
    }
}

void Socket::pluginTrigger(QString &data)
{
    //plugin.pluginKeyWord(data, mSocket);

}

void Socket::floodCheck(QString &data)
{
    antiflood->receiveLine(data, mSocket, &ignoredNicks);
}

void Socket::antiFloodSlot()
{

    //std::cout << "\nmySlot in GameLoop activated by singal from QTimer* timer; " << "\n";
    //antiflood->timeOutEvent();

}
