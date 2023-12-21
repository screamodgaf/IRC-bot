#include "controlirc.h"
#include "extract_nickandroom.h"
#include <QThread>
#include <termcolor.hpp>
ControlIRC::ControlIRC()
{

}



void ControlIRC::run(QString data_, std::string nick_, QTcpSocket *mSocket, ConsoleInput *consoleInput, std::string ownerNick)
{
    std::cout << termcolor::yellow<< "ControlIRC::run" << termcolor::reset<< std::endl;


    //ownerNick = e.extractNick(data_);

    std::string data = data_.toStdString();
    if(data.find("NOTICE " + nick_ + " :")!=std::string::npos &&
            data.find(ownerNick)!=std::string::npos)
    {
        Extract_NickAndRoom e;
        data = e.extractTextfromNotice(data_,nick_);

        //std::cout << "passed data is|: " << data << "|end\n";

        consoleInput->runFromIRC(data); // question, is there other way to use consoleInput without passing the pointer from socket class?
        data.clear();
        data_.clear();
        nick_.clear();
        nick.clear();

       }
}
