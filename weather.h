#ifndef WEATHER_H
#define WEATHER_H
#include <QDebug>
#include <QObject>
#include <QProcess>
class Weather: public QObject
{
    Q_OBJECT
public:
    explicit Weather(QObject *parent = nullptr);
    ~Weather( );
    void weatherKeyWord(QString data);
    void process(QString indexCore);
    void doPosting(std::string string);
private:
std::string room;

signals:
    void postSignal(QByteArray);
public slots:
};

#endif // WEATHER_H


