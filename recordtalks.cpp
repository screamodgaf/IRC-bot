#include "recordtalks.h"
#include <QCoreApplication>
// records chat to files - user gets separated file
// als records capitalism game
RecordTalks::RecordTalks(QObject *parent, CapitalismStorage* capitalismStorage_)
{
    capitalismStorage = capitalismStorage_;
    directory = QCoreApplication::applicationDirPath();
}

RecordTalks::~RecordTalks()
{
    std::cout << "destructor" << "\n";
}

void RecordTalks::run(QString data)
{
    std::cout << termcolor::yellow<< "RecordTalks::run" << termcolor::reset<< std::endl;


     std::cout << "whole data in RecordTalks::run DATA: " << data.toStdString() <<  + "\n";
    std::string text = data.toStdString();

    //dont record minion or sebot nicks
    if(text.find("UnwantedNick")!=std::string::npos || text.find("UnwantedNick")!=std::string::npos
        || text.find("UnwantedNick")!=std::string::npos || text.find("UnwantedNick")!=std::string::npos)
        return;

    extractNick(text);
    extractRoom(text);
    std::cout << "save in recordTalks is: " << save << "\n";
    if((nick.size()!=0 && room.size()!=0))
    {
        extractText(data);
        pushToVector(removeDigitsAndSpecials(nick), subText);
    }
    else
       return;
}

void RecordTalks::run()
{
    std::cout << "void RecordTalks::run()" << "\n";
    save=false;
    save = console.saveNowState();
    allTexts.reserve(6);

    if(save==true)
    {
        saveToFile(allTexts, "config/BOT.txt"); //trigger line saving to file
        file.close();
//        removeDuplicatedNicks();
//        createNickFiles(nicks_v);
//        match_nicks_and_allTexts(allTexts);
//        closeNickFiles();

        nicks_v.clear();
        nickAndFile_pair_vec.clear();
        text.clear();
        allTexts.clear();
        nick.clear();
        room.clear();
        subText.clear();
        save=false;
    }
}

void RecordTalks::extractText(QString &data)
{
    std::string text = data.toStdString(); // tu zmiana
    size_t pos, pos2;

    if((pos=text.find("PRIVMSG #"))!=std::string::npos)
    {
        if((pos2=text.find(":",pos))!=std::string::npos)
        {
            subText = text.substr(pos2+1, text.size()-(pos2+1)  );
            if((pos=subText.find("\u0001ACTION "))!=std::string::npos)
            {
                subText = subText.substr(pos+8 , subText.size()-3-(pos+8));
                 //std::cout << ">>> 1 subText: " << subText << "  nick: " << nick << " <<<" << "\n";
            }
            //else
               // std::cout << "subText: " << subText  << "\n"; - @@@@ to sie pojawia na czacie
        }
    }
    else  //if nothing to write to log:
    {
        return ;
    }
}

void RecordTalks::extractNick(std::string &text)
{
    Extract_NickAndRoom e;
    QString temp = QString::fromStdString(text);
    nick = e.extractNick(temp);
    //std::cout << "nick is: " << nick << "\n";

}

void RecordTalks::extractRoom(std::string &text)
{
    Extract_NickAndRoom e;
    QString temp = QString::fromStdString(text);
    room = e.extractRoom(temp);
    //std::cout << "room is: " << room << "\n";
}


void RecordTalks::pushToVector(std::string nick, std::string &subText) //general vector with texts
{
    if(subText.size()>25 ) //if line of text from irc is bigger than 25 signs
    {
        std::pair<std::string, std::string> para = std::make_pair(nick, subText);
        allTexts.push_back(para);  // pair --> vector

        emit textUpdateSignal(subText);
        std::cout << "EMIT RecordTalks::pushToVector emit textUpdateSignal: " <<  subText  << "\n";

    }

    if(allTexts.size()==60) //when the size of the vector gathering all the keyword matching lines is 60, save it to file
    {
        saveToFile(allTexts, "BOT.ext"); //trigger line saving to file
        file.close();
        ///makes files out of nicks:
//        removeDuplicatedNicks();
//        createNickFiles(nicks_v);
//        match_nicks_and_allTexts(allTexts);
//        closeNickFiles();

        nicks_v.clear();
        nickAndFile_pair_vec.clear();
        text.clear();
        allTexts.clear();
        nick.clear();
        room.clear();
        subText.clear();
        save=false;
    }
}

void RecordTalks::saveToFile( std::vector< std::pair<std::string, std::string> > &allTexts, std::string nickFileName)
{

    QString relativePath = directory.absoluteFilePath("config/BOT.txt");

//    std::string address = relativePath.toStdString() + nickFileName + ".txt";

    std::cout << "RecordTalks:: relativePath: " << relativePath.toStdString() << "\n";
//    std::cout << "RecordTalks:: address: " << address << "\n";

    file.open(relativePath.toStdString() , std::ios::out | std::ios::app);

    for (int i = 0; i < allTexts.size(); ++i)
    {
        file << allTexts[i].second; //writes text to file, not the nicks
    }
    std::cout << termcolor::on_red << "Saving to File"  << termcolor::reset<< std::endl;
}

void RecordTalks::createNickFiles(std::vector<std::string> &nicks_v)
{
    /* make vector of pair nick and file, so we can identify the file name and then
       put text-matching-to-file-nick with file name */


    QString relativePath = directory.absoluteFilePath("/logs/");


    for (int i = 0; i < nicks_v.size(); ++i)
    {
        std::fstream file2(relativePath.toStdString()+ nicks_v[i] + ".txt", std::ios::out | std::ios::app);

        std::pair<std::string, std::fstream> nickAndFile_pair;
        nickAndFile_pair = std::make_pair(removeDigitsAndSpecials(nicks_v[i]), std::move(file2));
        nickAndFile_pair_vec.push_back(std::move(nickAndFile_pair));
    }
}// nicks_v[i] put inside function removeDigitsAndSpecials(std::string& nick)


void RecordTalks::match_nicks_and_allTexts(std::vector< std::pair<std::string, std::string> > allTexts)
{
    for (int i = 0; i < nickAndFile_pair_vec.size(); ++i)
    {
        for (int j = 0; j < allTexts.size(); ++j)
        {
            if(nickAndFile_pair_vec[i].first == allTexts[j].first)
            {
                nickAndFile_pair_vec[i].second << allTexts[j].second;
                //nickAndFile_pair_vec[j].second.flush();
            }
        }
    }

    std::cout << "allTexts size in match_nicks_and_allTexts is: " << allTexts.size() << "\n";
}


void RecordTalks::removeDuplicatedNicks()
{
    std::vector< std::pair<std::string, std::string> > temp = allTexts;
    std::cout << "temp size: " << temp.size() << "\n";

    for (int i = 0; i < temp.size(); ++i)
    {
        for (int j = i+1; j < temp.size(); ++j)
        {
            if(temp[i].first == temp[j].first  &&  i+1< temp.size())
            {
                temp.erase(temp.begin()+j);
                j--;
            }
        }
    }


    for (int i = 0; i < temp.size(); ++i)
    {
        nicks_v.push_back(temp[i].first);
    }
    std::cout << "nicks_v.size: " << nicks_v.size() << "\n";

}


void RecordTalks::closeNickFiles() //closes separate nick files
{
    for (int i = 0; i < nickAndFile_pair_vec.size(); ++i)
    {
        nickAndFile_pair_vec[i].second.close();
    }
    std::cout << "Files closed..." << "\n";
}


std::string RecordTalks::removeDigitsAndSpecials(std::string& nick)
{
    for (int i = 0; i < nick.size(); ++i)
    {
        if(std::isdigit(nick[i]) && nick[i]!='_' ) //so aa_bbb is not saved as aabbb - so the '_' stays
        {

             nick.erase(nick.begin()+i); //remove 0 + 1 = 1 position
             std::cout << "removing in else if: " << nick[i] << "\n";
             i--;
        }
    }
    if(nick[nick.size()-1] == '_') //removes _ if its the last sign
        nick.erase(nick.size()-1);
    return nick;
}


