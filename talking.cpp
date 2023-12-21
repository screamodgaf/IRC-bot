#include "talking.h"
#include <fstream>
#include <algorithm>
#include <random>
#include <iostream>
// co console doesnt block?
std::string keyword = "lalalala";
Talking::Talking(QObject *parent)
{

}

void Talking::checkKeywordPresence(QString data, QTcpSocket *mSocket)
{
    std::cout << "Talking::checkKeywordPresence, keyword: " << keyword << "\n";
    //randomize the texts.txt
    auto rand = std::default_random_engine{};
    std::shuffle(std::begin(wholeLineVec), std::end(wholeLineVec), rand);

    //for(int i =0; i<wholeLineVec.size(); i++)
      //  std::cout << wholeLineVec[i] << "\n";



    for(int i=0; i<wholeLineVec.size(); ++i)
    {
        if(wholeLineVec[i].find(keyword)!=std::string::npos)
        {
            std::string matchingLine = wholeLineVec[i];

            size_t separatorIndx = matchingLine.find("|");
            std::string core = matchingLine.substr(separatorIndx+2,
                                                   matchingLine.length()-(separatorIndx+2));;

            doPosting(core ,mSocket);
            matchingLine.clear();
            std::cout << "2@@@@@@ - core: " <<core << "\n";
            core.clear();
            break;
            return;
        }
    }
}

void Talking::getText()
{
    std::string textLine;
    size_t indexOfSeparator;
    std::ifstream file("config/BOT.txt" );

    while(getline(file, textLine))
    {
        //std::cout <<"text: " <<textLine << "\n";
        if((indexOfSeparator = textLine.find("|"))!=std::string::npos)
        {
            std::string preSeparator = textLine.substr(0,indexOfSeparator);
            //std::cout << preSeparator << "\n";
            std::string postSeparator =
                    textLine.substr(indexOfSeparator+1, textLine.length()-(indexOfSeparator+1));
            //std::cout << postSeparator << "\n";


            //if keyword not in vector:
            auto it = std::find(keywordsVec.begin(), keywordsVec.end(), preSeparator);
            if(it == keywordsVec.end())
            {
                keywordsVec.push_back(preSeparator);
            }
            wholeLineVec.push_back(textLine);
        }
    }
    file.close();
}

void Talking::doPosting(std::string &lineToPost, QTcpSocket *mSocket)
{
    lineToPost = "PRIVMSG " +room +" "+ nick + " :" + lineToPost+ "\r\n\ ";
    //lineToPost = "PRIVMSG " +room + " " + lineToPost+ "\r\n\ ";
std::cout << "LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOL \n";
    const char *send = lineToPost.c_str();
    mSocket->write(send);
    mSocket->flush();
    lineToPost.clear(); //clear before next post
    mSocket->flush();
    mSocket->reset();

    std::cout<< "3@@@@@@ - lineToPost\n"  ;
    return;
}
