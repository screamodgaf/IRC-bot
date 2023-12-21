#include "extract_nickandroom.h"
#include <QDebug>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <termcolor.hpp>
Extract_NickAndRoom::Extract_NickAndRoom()
{
    std::cout << "Extract_NickAndRoom::Extract_NickAndRoom()" << std::endl;
    nick = "";
    room = "";
    subText ="";
}


///this works:
//std::string Extract_NickAndRoom::extractNick(QString& data)
//{
//    qDebug() << "Extract_NickAndRoom::extractNick DATA: " << data;
//    std::string text = data.toStdString();
//    if(text.find(":")!=std::string::npos &&
//       text.find("!")!=std::string::npos)
//    {
//            size_t beginning = text.find_first_of(":");
//            size_t exclamation = text.find_first_of("!",beginning);
//            nick = text.substr(beginning+1, exclamation-(beginning+1));

//            size_t pos;
//            if((pos = nick.find("@"))!=std::string::npos ||
//               (pos = nick.find("%"))!=std::string::npos ||
//               (pos = nick.find("+"))!=std::string::npos ||
//               (pos = nick.find("!"))!=std::string::npos ||
//               (pos = nick.find("~"))!=std::string::npos)
//                nick = nick.erase(pos);
//            //std::cout << nick << "\n";

//    }
//    std::cout << "nick: " <<  nick << "\n";
//    return nick;

//}

std::string Extract_NickAndRoom::extractNick(QString& data)
{
    std::cout << "Extract_NickAndRoom::extractNick DATA: " << data.toStdString() << "\n";
    size_t beginning =0;
    size_t nickEnd1  =0;
    size_t nickEnd2  =0;
    size_t nickEnd  =0;
    std::string text = data.toStdString();
    if((beginning=text.find(":"))!=std::string::npos)
    {

        if((nickEnd1=text.find("!", beginning))!=std::string::npos)
            ;
        if((nickEnd2=text.find(" ", beginning))!=std::string::npos)
            ;

        if(nickEnd1<nickEnd2) //nick may end with "!" or " " but also those 2 can be part of message, so the one that is closer to the # is the nick ending
            nickEnd = nickEnd1;
        else if(nickEnd2<nickEnd1)
            nickEnd = nickEnd2;


        {
            std::cout << "beginning: " << beginning << "\n";
            std::cout << "nickEnd: " << nickEnd << "\n";
            nick = text.substr(beginning+1, nickEnd-(beginning+1));
        }


        size_t pos;
        if((pos = nick.find("@"))!=std::string::npos ||
                (pos = nick.find("%"))!=std::string::npos ||
                (pos = nick.find("+"))!=std::string::npos ||
                (pos = nick.find("!"))!=std::string::npos ||
                (pos = nick.find("~"))!=std::string::npos)
            nick = nick.substr(pos, nick.size()-pos);
    }

    std::cout << "extractNick: " <<  nick << "\n";
return nick;

}


std::string Extract_NickAndRoom::extractRoom(QString &data)
{
    std::cout << "Extract_NickAndRoom::extractRoom(QString &data): " << data.toStdString() << "\n";
    std::string text = data.toStdString();
    size_t posHash=0;
    size_t posColon=0;
    std::string room;
    bool privMSGisFound=false;
    bool colonisFound = false;
    if((posHash=text.find_first_of("#"))!=std::string::npos)
    {
        //std::cout << posPRIVMSG << "\n";
        privMSGisFound=true;
    }

    if((posColon=text.find_first_of(" ", posHash))!=std::string::npos)
    {
        //std::cout << posColon << "\n";
        colonisFound = true;
    }

    if(privMSGisFound==true && colonisFound==true)
    {


        room=text.substr(posHash ,  posColon - posHash  );
        //std::cout << room  << "\n";
    }

    std::cout <<  "room: " << room << "\n";
    return room;
}

std::string Extract_NickAndRoom::extractRoomFromJoin(const QString &data)
{
    std::cout << "Extract_NickAndRoom::extractRoomFromJoin" << "\n";
    std::string text = data.toStdString();
    std::string temp = "";
    size_t posBeg = 0;
    size_t posEnd = 0;
    if((posBeg = text.find("JOIN :#"))!= std::string::npos &&
        text[0] == ':')
    {
        posBeg+=6;

        temp  = text.substr(posBeg, posEnd-posBeg);
        temp.resize(temp.size()-2); // for \r\n";
        return temp;
    }
    else
        return ""; //returns empty



}

std::string Extract_NickAndRoom::extractRoomFromDeparture(const QString &data)
{
    std::string text = data.toStdString();
    std::string temp = "";
    size_t posBeg = 0;
    size_t posEnd = 0;
    if((posBeg = text.find("PART #"))!= std::string::npos
            && text.find("PRIVMSG #")==std::string::npos)
    {
        posBeg+=6;

        if((posEnd = text.find_first_of(' ',posBeg))!= std::string::npos){
            temp  = text.substr(posBeg, posEnd - posBeg);
            return temp;
        }

    }
    else
        return ""; //returns empty
}



std::string Extract_NickAndRoom::extractText(QString &data)
{
    std::cout << "Extract_NickAndRoom::extractRoom(QString &data): "<<  data.toStdString() << "\n";

    std::string text = data.toStdString();
    size_t pos, pos2, pos3;
    std::string subText;
    if((pos=text.find("PRIVMSG #"))!=std::string::npos)
    {
        if((pos2=text.find(":",pos))!=std::string::npos)
        {
            if((pos3=text.find("\r",pos2))!=std::string::npos ||
                    (pos3=text.find("\n",pos2))!=std::string::npos) //n too?
                subText = text.substr(pos2+1,pos3-(pos2+1));
        }


        if((pos3=subText.find("\u0001ACTION "))!=std::string::npos)
        {
            subText = subText.substr(pos3+8 , subText.size()-3-(pos3+8));
        }
        std::cout << "subText: " << subText<< "\n";
        return subText;
    }
    else{
        std::cout << "subText: IS EMPTY" << "\n";
        return "";
    }
}


std::string Extract_NickAndRoom::extractTextWithoutN(QString &data)
{
    std::string text = data.toStdString();
    size_t pos, pos2, pos3;
    std::string subText;
    if((pos=text.find("PRIVMSG #"))!=std::string::npos)
    {
        if((pos2=text.find(":",pos))!=std::string::npos)
        {
            subText = text.substr(pos2+1,text.size()-(pos2+3));//1
        }
        return subText;
    }
    else
        return "";
}

std::string Extract_NickAndRoom::extractCleanedCommandWithoutN(QString &data)
{
    std::string text = data.toStdString();
    size_t pos, pos2, pos3;
    std::string subText;
    if((pos=text.find("PRIVMSG #"))!=std::string::npos)
    {
        if((pos2=text.find(":",pos))!=std::string::npos)
        {
            subText = text.substr(pos2+1,text.size()-(pos2+3));//1
        }

        std::stringstream ss; // we only need first word - the command
        ss << subText;
        for (int i = 0; i < 1; ++i) {
            ss >>subText;
        }
        return subText;
    }
    else
        return "";
}

std::string Extract_NickAndRoom::extractTextfromNotice(QString &data_, std::string &nick)
{

    std::string data = data_.toStdString();
    size_t pos;
    std::string textReturn;
    std::string searched = "NOTICE " + nick + " :";
    if((pos=data.find(searched))!=std::string::npos)
    {
        textReturn = data.substr(pos + searched.size(), data.size()-(pos + searched.size()));
        std::cout << textReturn << "\n";

    }
    return textReturn;
}

std::vector<std::string> Extract_NickAndRoom::extractCommandElements(QString data_, std::string command)
{

    std::string data = data_.toStdString();

    size_t pos, pos2;
    std::string subText;
    std::vector<std::string> assetElements_v;

    if( (pos  = data.find(command)) ==std::string::npos &&
            (pos2 = data.find("\r\n",pos + command.size())) ==std::string::npos &&
            pos2 - (pos + command.size()+1)<=0)
    {
        /*return empty vector that will cause atomizeCommand() = false only when
          if in data command is not found, if "\r\n" is not found there
          and if command touches escape: "command\r\n" - cause then SIZE of element would be ZERO and it causes CRASH!
        */
        std::cout << termcolor::blue << "return assetElements_v;" << termcolor::reset<< std::endl;
        return assetElements_v;
    }
    subText = data.substr(pos + command.size()+1, pos2 - (pos + command.size()+1) );
    //std::cout << "sub: " <<  subText << std::endl;

    std::stringstream ss;
    ss <<subText;
    std::string word;
    while(ss >> word){
        assetElements_v.push_back(word);
    }
    std::cout << "Extract_NickAndRoom::extractCommandElements assetElements_v.size(): " << assetElements_v.size() << std::endl;
    return assetElements_v;
}


std::string Extract_NickAndRoom::round_float_fToS(float money)
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << money;
    return ss.str();
}


std::string Extract_NickAndRoom::reduceZerosInFloatString(std::string i)
{
    std::cout << termcolor::on_cyan << "i: " << i << termcolor::reset<< std::endl;
    std::cout << "reduceZerosInFloatString: " << i  << std::endl;
    //instead of "emo 10.000000 farbyka" we get "emo 10.00 farbyka"
    std::stringstream ss;
    std::vector<std::string> temp_v;
    ss <<i; //whole line of "emo 10.000000 farbyka" we put into sstring
    std::string s;
    while (ss >> s) {
        temp_v.push_back(s);
    }

    std::string str_temp = temp_v[0] + " " ; //nick

    str_temp += round_float_fToS(std::stof(temp_v[2])) + " "; //money

    for (int i = 3; i < temp_v.size();   i++ ) {
        // if(i)
        str_temp+= temp_v[i] + " ";
    }
    return str_temp;
}
