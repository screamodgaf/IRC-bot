#include "plugin.h"
#include <QDebug>

Plugin::Plugin(QObject *parent) : QObject(parent)
{

}
void Plugin::mi1()
{
    //QTimer::singleShot(5000, this , SLOT(mi2()));
}
void Plugin::mi2()
{
     std::cout << "mumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumumu" << "\n";

}




