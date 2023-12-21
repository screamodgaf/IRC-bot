#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>
#include "extract_nickandroom.h"
#include <QTcpSocket>
#include <QTimer>




class Plugin : public QObject
{
    Q_OBJECT
public:
    explicit Plugin(QObject *parent = nullptr);
    void mi1();
public slots:
    void mi2();

private:


};

#endif // PLUGIN_H
