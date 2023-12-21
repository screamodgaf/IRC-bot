#ifndef ENGLISHPOSTS_H
#define ENGLISHPOSTS_H
#include <QObject>
#include "loadlogs.h"
#include "extract_nickandroom.h"
#include <QTimer>
#include <QMutex>
class EnglishPosts : public QObject
{
    Q_OBJECT
public:
    explicit EnglishPosts(QObject *parent = nullptr);
    ~EnglishPosts();
    void doPosting(int index);
    void doPostingOnRequest(std::string& englishLine_);
    void checkKeyword(QString data_, QTimer* timerEnglish, int englishTimerFrequency);
    void fillNullPointer(LoadLogs* loadLogs_, QMutex* m);
private:


private:
    std::vector<std::string> englishDatabase;
    int indexEnglish;
    QString data;
    LoadLogs *loadLogs;
    Extract_NickAndRoom extract;
    std::string englishLine;
    std::string room ="";
    QMutex *mutex;

private:
    void postToIRC(std::string newsText , std::string desctiption);
    void pickEnglishLine();
    void doPostingOnRequest();

signals:
    void postSignal(QByteArray);
public slots:
};

#endif // ENGLISHPOSTS_H
