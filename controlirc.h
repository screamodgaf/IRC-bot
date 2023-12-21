#ifndef CONTROLIRC_H
#define CONTROLIRC_H
#include <QTcpSocket>
#include "consoleinput.h"
class ControlIRC
{
public:
    ControlIRC();
    void run(QString data_, std::string nick_, QTcpSocket *mSocket, ConsoleInput *consoleInput, std::string ownerNick);
private:
    std::string data;
    std::string nick;
};

#endif // CONTROLIRC_H
