#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H
#include <QObject>
#include <QThread>
#include "optionsactivator.h" //for option activator

class ConsoleInput: public QObject
{
    Q_OBJECT

public:

    ConsoleInput(QObject *parent = nullptr);
    void startConsole(QThread *thread_);

    void determineOption(std::string input);
    void runFromIRC(std::string &data);
signals:
    void mySignal(const QByteArray);
    //void mySignalIRC(const QByteArray);
    void saveSignal();
    void sayNow();


public slots:
    void run();
    bool weatherState();
    bool recordTalkState();
    bool newsState();
    bool saveNowState();
    void set_saveNow_is_toFalse();
    bool botSpeechState();
    bool englishPostsState();
    bool greetingsState();
    bool capitalismState();
private:
    void postInput(std::string &input);
    void postRawInput(std::string &input);

    QThread *thread;
    OptionsActivator *optionsActivator;

private:
    static bool weather_is;
    static bool news_is;
    static bool recordTalk_is;
    static bool saveNow_is;
    static bool botSpeech_is;
    static bool english_is;
    static bool greetings_is;     //for console turn off
    static bool capitalismGame_is;


};

#endif // CONSOLEINPUT_H



