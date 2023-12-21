#include "stockmarket.h"
#include <QProcess>
#include <iostream>
#include <termcolor.hpp>
#include <sstream>
#include <QDir>
#include <QCoreApplication>
#include <iostream>
#include <QDebug>
StockMarket::StockMarket(QObject *parent) : QObject(parent)
{

}

StockMarket::~StockMarket()
{
    std::cout << termcolor::green << "Destructor StockMarket: " << termcolor::reset<< std::endl;

}

std::vector<std::string> StockMarket::process(std::string& indexCore)
{
    // try {

    std::cout << termcolor::red << "StockMarket::process - indexCore: " << indexCore << termcolor::reset<< std::endl;
    std::cout << "StockMarket::process" << std::endl;


    QDir directory = QCoreApplication::applicationDirPath();
    QString relativePath = directory.absoluteFilePath("config/finances.py");




//    QString cmdCommand = "python3";
    QString cmdCommand = "python3";
    QStringList list;
    list << relativePath << QString::fromStdString(indexCore);
    std::cout << "indexCore: " << indexCore + "\n";
    std::cout <<  "relativePath: " << relativePath.toStdString() + "\n";
    qDebug() << "stockmarket - cmdCommand: " << cmdCommand;
    qDebug() << "stockmarket -  list: " << list;
    QProcess *qprocess= new QProcess();
    qprocess->setProgram(cmdCommand);
    qprocess->setArguments(list);
    qprocess->start();
    qprocess->waitForFinished(15000);

    std::cout << "while(qprocess->canReadLine())" << "\n";
    while(qprocess->canReadLine())
    {
        QByteArray tempByteArr = qprocess->readLine();
        std::string temp = tempByteArr.toStdString();
        std::cout << "qprocess temp: " << temp << std::endl;

        std::stringstream ss; // remove end line from temp:
        ss << temp;
        for (int i = 0; i < 1; ++i) {
            ss >>temp;
        } // now temp is without endline


        stockAndPrice_v.push_back(temp);
        std::cout << "stockAndPrice_v: " << stockAndPrice_v.size() << "\n";
    }
    qprocess->close();
    qprocess = nullptr;
    delete qprocess;
    std::cout << "stockAndPrice_v.size(): " << stockAndPrice_v.size() << std::endl;

    // } catch (...) {
    //std::cout << termcolor::on_red << "ERROR" << termcolor::reset<< std::endl;
    // }
    return stockAndPrice_v;
}
