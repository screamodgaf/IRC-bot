#include <QCoreApplication>
#include "socket.h"
#include "speech.h"
#include <fstream>
#include "optionsactivator.h"
#include "settings.h"




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    OptionsActivator opt;
    //bot:
    Socket socket(Settings::getProxyIP() , Settings::getProxyPort() ,  Settings::getMyNick());

    return a.exec();
}

