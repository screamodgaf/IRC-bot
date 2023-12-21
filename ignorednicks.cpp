#include "ignorednicks.h"
#include <iostream>
#include <termcolor.hpp>

IgnoredNicks::IgnoredNicks()
{
    std::cout << termcolor::red << "IgnoredNicks Ctor" << termcolor::reset<< std::endl;
}

bool IgnoredNicks::checkForIgnoredNicks(QString data)
{

    std::cout << "IgnoredNicks::checkForIgnoredNicks(QString data)" << std::endl;
    std::string nick = e.extractNick(data);

    if(nick.size() == 0)
        return false;
    for (auto &i : ignoredNicks_v)
    {
        std::cout << termcolor::red << "ignoredNicks_v: " << i << termcolor::reset<< std::endl;
        if(i == nick){
            //std::cout << termcolor::crossed << "found ignored nick: " << nick << termcolor::reset<< std::endl;
            return true;
        }
    }
    return false;
}


void IgnoredNicks::fill_ignoredNicks_v()
{
    //gets filled every each readyRead() so its not adding stuff all the time
    ignoredNicks_v = {"UnwantedNick", "UnwantedNick", "UnwantedNick", "UnwantedNick", "UnwantedNick" }; //own nick should be here
    //std::cout << termcolor::blue << "ignoredNicks_v.size(): " << ignoredNicks_v.size() << termcolor::reset<< std::endl;

}

void IgnoredNicks::addToIgnore(std::string nick)
{
    std::cout << termcolor::red << "Added to ignore" << termcolor::reset<< std::endl;
    ignoredNicks_v.push_back(nick);
}

void IgnoredNicks::Unignore(std::string nick)
{

    std::cout << termcolor::yellow << "IgnoredNicks::Unignore" << termcolor::reset<< std::endl;
    ignoredNicks_v.erase(std::remove(ignoredNicks_v.begin(), ignoredNicks_v.end(), nick), ignoredNicks_v.end());
}
