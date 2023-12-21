#include "botspeech.h"
#include <sstream>
#include <iostream>
#include "extract_nickandroom.h"
#include <ctime>
#include <utility>
#include <random>
#include "colourmanipulation.h"

#include "removedoublenicks.h"
//normal talk
BotSpeech::BotSpeech(QObject *parent): QObject(parent)
{

}

void BotSpeech::runBotSpeech(QString &data, std::vector<std::string> &database, LoadLogs *l_ptr, bool empty)
{
    std::cout << termcolor::yellow<< "BotSpeech::runBotSpeech" << termcolor::reset<< std::endl;

    l_ptr_ = l_ptr;
    std::srand(time(nullptr));
    std::string temp = data.toStdString();
    Extract_NickAndRoom ext;

    std::random_device rd;
    std::mt19937 g(rd());

   std::shuffle(database.begin(), database.end()-20, g); //additional shuffling (except the last 15 lines)


    if(empty==false)
    {
        nick = ext.extractNick(data);
        room = ext.extractRoom(data); //dodane
        if(nick == "unwantedNick")
            return;
    }
    else // on /say:
    {
        nick = "";

        room = ext.extractRoom(data); //dodane
        //auto r = std::rand()% database.size() -1;

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<> distr(0, database.size()-20); // define the range | -15 so wont shuffle ast 15 enties so wont repeat them after user rightr away

        doPosting(database[distr(eng)]);

    }
    room = ext.extractRoom(data);
    text = ext.extractText(data);

    std::cout << "text2: "  << text << "|\n";
    if(room.size()==0 || text.size()<2)
        return;

    longestWord = determineLongestWord(text);
    //matchKeyword_withDatabase(longestWord);
    std::string line = "";
    size_t index = 0;
    std::pair<std::string, size_t> MatchingLineAndIndex;
    if(longestWord.size() >0)
    {
        MatchingLineAndIndex = matchKeyword_withDatabase(longestWord, database);
        line  = MatchingLineAndIndex.first;
        index = MatchingLineAndIndex.second; //line's index in database
        //database[index] << " -----= " line for sure
    }


    //move line to the end of the database, so when someone writes the keyword again, the bot wont say the same line again


    //database.erase(database.begin()+index);

    // i cant swap with the last element cause then with the next search it will reswap
    //SWAPPING :


    //    std::random_device rd; // obtain a random number from hardware
    //    std::mt19937 eng(rd()); // seed the generator
    //    std::uniform_int_distribution<> distr(index, database.size()-6); // define the range
    //    auto r = distr(eng);
    auto r = database.size()/2 +  (std::rand()% (database.size() - database.size()/2)); // generate number
    std::cout << "R: " << r << std::endl;

    std::string tempSwap = database[index];
    database[index] = database[r];
    database[r] = tempSwap;


    //database.erase(database.begin()+ index); // better not to delete cause indexes change in vector

    std::cout << "0 LINE FROM DATABASE: " << line << std::endl;
    doPosting(line);
}

std::pair<std::string, size_t> BotSpeech::matchKeyword_withDatabase(std::string longestWord, std::vector<std::string> &database)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(database.begin(), database.end() - 20, g); // - 20 so the bot wont repear one of the last 6 longest word contaner sentences
    std::cout << "------RANDOOOM[0]------- " << "\n";
    std::cout << "a longest word: " << longestWord << std::endl;
    std::string longestWord_space = longestWord + " ";
    std::string st1_longestWord = longestWord;

    std::string nd2_longestWord = "";

    std::string nd3_longestWord = "";





    std::cout << "b longestWord.size(): "  << longestWord.size() << std::endl;



    if(longestWord.size()>4)
        nd2_longestWord = removeEnds(longestWord, 1); //shortening by 1 sign at the end piosenka = piosenk

    if(longestWord.size()>5)
        nd3_longestWord = removeEnds(longestWord, 2); //shortening by 2 sign at the end piosenka = piosen

    std::cout << "d past if(longestWord.size()>4)";
    std::pair<std::string, size_t> returnLine;
    size_t pos = 0;
    //std::string returnLine;
    std::vector<std::pair<std::string, size_t>> linesWith_st1_longestWord_v;
    std::vector<std::pair<std::string, size_t>> linesWith_st2_longestWord_v;
    std::vector<std::pair<std::string, size_t>> linesWith_st3_longestWord_v;
    std::cout << "before for loop | database.size: " << database.size();
    //error
    for (auto i = 0; i < database.size()-20; ++i)   // - 20 so the bot wont repear one of the last 6 longest word contaner sentences
    {

        if(database[i].find(longestWord_space) !=std::string::npos) //find piosenka" "
        {
            //cant RETURN here cause i might still find whole line with uncut line
            return  std::make_pair(database[i], i);
        }
        // if throughout whole database there was no full piosenka" " then the additional conditions will be returned:

        if(linesWith_st1_longestWord_v.size()==0)
            if(database[i].find(st1_longestWord)!=std::string::npos)   //find piosenka
            {
                linesWith_st1_longestWord_v.push_back(std::make_pair(database[i], i));
                returnLine = std::make_pair(database[i], i);
            }else if (st1_longestWord.size()<=2) //safety when size is only 2 so use random
            {
                std::cout << "1 \n";
                std::cout<< "st1_longestWord.size() 2 : "<< st1_longestWord.size() << "\n";
                std::cout << "2 \n";
                std::cout<< "int r = std::rand()% (database.size() -6);" << "\n";
                std::cout << "3 \n";
                int r = std::rand()% (database.size() -10);
                //std::cout << "Random int is: " << r << "\n";
                std::cout << "4 \n";
                std::cout<< "return std::make_pair(database[r], r): " << "\n";
                return std::make_pair(database[r], r);

            }

        //seek to fill linesWith_st2_longestWord_v if linesWith_st2_longestWord_v is empty and st1 is also empty
        if(linesWith_st1_longestWord_v.size()==0 && linesWith_st2_longestWord_v.size()==0)
            if(database[i].find(nd2_longestWord)!=std::string::npos)  //find piosenk
            {
                linesWith_st2_longestWord_v.push_back(std::make_pair(database[i], i));
                returnLine = std::make_pair(database[i], i);
            }

        //seek to fill linesWith_st3_longestWord_v if linesWith_st3_longestWord_v is empty and st2 and st1 are also empty
        if(linesWith_st1_longestWord_v.size()==0 && linesWith_st2_longestWord_v.size()==0
                && linesWith_st3_longestWord_v.size()==0)
            if(database[i].find(nd3_longestWord)!=std::string::npos) //find piosen
            {

                linesWith_st3_longestWord_v.push_back(std::make_pair(database[i], i));
            }

    }//for ends

    if(linesWith_st1_longestWord_v.size()!= 0){
        std::cout << " if(linesWith_st1_longestWord_v.size()!= 0)\n";
        return linesWith_st1_longestWord_v[0];
    }


    if(linesWith_st2_longestWord_v.size()!= 0){
        std::cout << "if(linesWith_st2_longestWord_v.size()!= 0)\n";
        return linesWith_st2_longestWord_v[0];
    }

    if(linesWith_st3_longestWord_v.size()!= 0){
        std::cout << " if(linesWith_st3_longestWord_v.size()!= 0\n";
        return linesWith_st3_longestWord_v[0];
    }

    //if no match found, then use random:

    int r = std::rand()% (database.size() -20);
    //std::cout << "Random int is: " << r << "\n";


    return std::make_pair(database[r], r);
}

std::string BotSpeech::determineLongestWord(std::string &text)
{

    if(text.size()>0)
    {
        std::stringstream stream(text);
        std::string buffor;
        std::vector<std::string> line;

        while (stream>>buffor)
            line.push_back(buffor);

        std::string longest_word = line[0];

        for (auto i = 0; i < line.size(); ++i)
        {
            if((i+1)<line.size() && longest_word.size() < line[i+1].size())
                longest_word = line[i+1];
        }
        return longest_word;
    }
    else
        return "";
}

std::string BotSpeech::removeEnds(std::string longestWord, size_t n)
{
    longestWord = longestWord.erase(longestWord.size()-n, longestWord.size()-(longestWord.size()-n));
    //std::cout << longestWord << "\n";
    return longestWord;
}

void BotSpeech::doPosting(std::string &lineToPost_)
{
    lineToPost = lineToPost_;
    //std::cout << "BEFORE ENTERING COLOURS: " << lineToPost << std::endl;


    //load vector of nicks from files l.getNicksFromLogs()

    //LoadLogs l; - cant use it like this, cause each call for the method would create another LoadLogs object and reset it calling destructor
    std::vector<std::string> nicks_v = l_ptr_->getNicksFromLogs();

    std::cout << "@@@ l.getNicksFromLogs(): " << nicks_v.size() << std::endl << std::endl;


    RemoveDoubleNicks r;
    lineToPost = r.removeNicksFromBottxt(lineToPost, nicks_v);

    ColourManipulation c;

    std::string allTimeColour = "\x03""12";

    //lineToPost = "PRIVMSG " +room +  " :" + allTimeColour + c.addColours(lineToPost) + "\r\n"; // with nick
    //    lineToPost = "PRIVMSG " +room +" "+ nick + " " + allTimeColour + c.addColours(lineToPost) + "\r\n"; // with nick

    std::cout << "room: " << room << "\n";


    std::cout << termcolor::on_yellow << "botSpeech lineToPost: " << lineToPost << termcolor::reset<< std::endl;
    //    lineToPost = "PRIVMSG " +room +" :"  +   allTimeColour + c.addColours(lineToPost) + "\r\n"; // with nick
    lineToPost = "PRIVMSG " +room +" :"  + nick + " " + "\x03""12" + lineToPost  + "\r\n"; //works

    //these r n on linux are not needed! (bot stops talking)
    //    lineToPost.erase(std::remove(lineToPost.begin(), lineToPost.end(), '\n'), lineToPost.end());
    //    lineToPost.erase(std::remove(lineToPost.begin(), lineToPost.end(), '\r'), lineToPost.end());


    QTimer::singleShot(5000, this, SLOT(speakNowAfter5sec()));
}

void BotSpeech::speakNowAfter5sec()
{
    QByteArray temp = QByteArray::fromStdString(lineToPost);
    emit postSignal(temp);

}



