#ifndef BOTSPEECH_H
#define BOTSPEECH_H
#include <QThread>
#include <QObject>
#include <vector>
#include "loadlogs.h"
#include <QTimer>
#include <algorithm>
// this is normal speech with 5 sec delay

class BotSpeech: public QObject
{
    Q_OBJECT
public:
    explicit BotSpeech(QObject *parent = nullptr);
    void runBotSpeech(QString &data, std::vector<std::string> &database, LoadLogs *l_ptr, bool empty);

public slots:
    void speakNowAfter5sec();

private:
    std::string determineLongestWord(std::string &text);
    std::string removeEnds(std::string longestWord, size_t n);
    std::pair<std::string, size_t>  matchKeyword_withDatabase(std::string longestWord, std::vector<std::string> &database);
    void doPosting(std::string &lineToPost_);
signals:
    void postSignal(const QByteArray);

private:
    std::string lineToPost;
    std::string nick;
    std::string room;
    std::string text;
    std::string longestWord;
    LoadLogs* l_ptr_;
    QString send;
};

#endif // BOTSPEECH_H
