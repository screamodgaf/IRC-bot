#ifndef TALKING_H
#define TALKING_H
#include <QObject>
#include <QTcpSocket>
#include <QThread>


class Talking: public QObject
{
    Q_OBJECT

public:
    Talking(QObject *parent = nullptr);
    void checkKeywordPresence(QString data, QTcpSocket *mSocket);
    void getText();
    void doPosting(std::string &lineToPost, QTcpSocket *mSocket);

private:
   // QTcpSocket *socket;
    std::vector<std::string> keywordsVec;
    std::vector<std::string> wholeLineVec;

    std::string nick;
    std::string room;
};

#endif // TALKING_H
