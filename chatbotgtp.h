#ifndef CHATBOTGTP_H
#define CHATBOTGTP_H

#include <QDebug>
#include <QObject>
#include <QProcess>
class ChatbotGTP: public QObject
{
    Q_OBJECT
public:

    explicit ChatbotGTP(QObject *parent = nullptr);
    ~ChatbotGTP();
    void chatbotKeyWord(QString data);
    void process(QString indexCore);
    void doPosting(std::string string);
    bool checIgnored(const QString& data);
private:
std::string room;
std::string nick;
//std::string& insertNewLines(std::string& string);
std::vector<std::string>  separateLine(std::string string);

QString lastMessage;

signals:
    void postSignal(QByteArray);
public slots:
};

#endif // CHATBOTGTP_H
