#ifndef SPEECH_H
#define SPEECH_H
#include <QTcpSocket>
#include <QObject>
#include <vector>
#include <QString>

class LoadLogs; //forward declaration

class Speech
{
public:
    Speech(LoadLogs &loadLogs);
    void checkKeywordPresence(QString &data);
    void getKeywordsAndText();
private:

    void doPosting(std::string &lineToPost);
    void matchKeyToPost(std::string &keyword);


    QTcpSocket *socket;
    std::vector<std::string> keywordsVec;
    std::vector<std::string> wholeLineVec;

    LoadLogs* loadlogs_;
    std::string nick;
    std::string room;
};



#endif // SPEECH_H
