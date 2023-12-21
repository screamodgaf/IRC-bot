// predefined texts to post in texts.txt
#include "speech.h"
#include <algorithm>
#include <random>
#include <iostream>
#include "extract_nickandroom.h"
#include <ctime>
#include "loadlogs.h"

Speech::Speech(LoadLogs& loadLogs)
    :loadlogs_{&loadLogs}
{
}

void Speech::checkKeywordPresence(QString& data)
{
    std::cout << termcolor::yellow<< "Speech::checkKeywordPresence" << termcolor::reset<< std::endl;

    Extract_NickAndRoom extract;

    room = extract.extractRoom(data);
    nick = extract.extractNick(data);
    std::cout << "Speech::checkKeywordPresence" << std::endl;
    if(room.size() == 0 || nick.size() == 0)
        return;
    std::cout << "checkKeywordPresence starts" <<  "\n";
    //check every time message from chat comes, if keyword is present:
    std::string dataStr =data.toStdString(); //chat line

     //for(int i =0; i<keywordsVec.size(); i++)
      //std::cout << keywordsVec[i] << "\n";


    for(int i =0; i<keywordsVec.size(); i++)
    {
        if(dataStr.find(keywordsVec[i])!=std::string::npos)
        {
            //match found
             std::cout << "czat line: " << dataStr<<"\n";;
             std::cout << "1@@@@@@ - MATCH FOUND!!!!!!!!!!!!-> " <<keywordsVec[i] <<"\n";
             matchKeyToPost(keywordsVec[i]);


             data.clear();
             dataStr.clear();

             break;   //so it doesnt post all finds
            // return;

        }
    }
}


void Speech::matchKeyToPost(std::string &keyword)
{
    std::cout << "found keyboard is: " << keyword << "\n";
    //randomize the texts.txt
    //auto rand = std::default_random_engine{};
    //std::shuffle(std::begin(wholeLineVec), std::end(wholeLineVec), rand);


    //wholeLineVec contains all texts.txt
    std::vector<std::string> LinesWithMatchingKeyword_v;
    LinesWithMatchingKeyword_v.reserve(20);

    for(int i=0; i<wholeLineVec.size(); ++i)
    {
        if(wholeLineVec[i].find(keyword)!=std::string::npos)
        {
            LinesWithMatchingKeyword_v.push_back(wholeLineVec[i]); //now filled with matching
            std::cout << "wholeLineVec[i]: " << wholeLineVec[i] << std::endl;
            std::cout << "LinesWithMatchingKeyword_v SIZE: " << LinesWithMatchingKeyword_v.size() << std::endl;
        }
    }
            std::cout << "random" << std::endl;
            auto r = std::rand()%(LinesWithMatchingKeyword_v.size() ); //random from 0 to size, cant be size-1 cause 
            std::cout << "R: " << r << std::endl;
            std::cout << "LinesWithMatchingKeyword_v[r]: " << LinesWithMatchingKeyword_v[r] << std::endl;
            std::string matchingLine = LinesWithMatchingKeyword_v[r];

            //std::cout << "R: " << r << " size: " << LinesWithMatchingKeyword_v.size() << std::endl;

            size_t separatorIndx = matchingLine.find("|");
            std::string core = matchingLine.substr(separatorIndx+2,
                                                   matchingLine.length()-(separatorIndx+2));;

            doPosting(core);
            matchingLine.clear();
            std::cout << "2@@@@@@ - core: " <<core << "\n";
            core.clear();

            return;
}



void Speech::doPosting(std::string &lineToPost )
{

//     lineToPost = "PRIVMSG " +room +" "+ nick + " :" + lineToPost+ "\r\n\ ";

//     const char *send = lineToPost.c_str();
//     mSocket->write(send);
//     mSocket->flush();
//     lineToPost.clear(); //clear before next post
//     mSocket->flush();
//     mSocket->reset();

//     qDebug() << "3@@@@@@ - lineToPost"  ;
//     return;
}


void Speech::getKeywordsAndText()
{
    std::string textLine;
    size_t indexOfSeparator;


   // while(getline(loadLogs_, textLine))
    for (std::string textLine : *loadlogs_->textsGetter()){
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
}


    //srand(unsigned(time(nullptr)));
    //std::random_shuffle(wholeLineVec.begin(), wholeLineVec.end());



