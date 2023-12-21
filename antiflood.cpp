#include "antiflood.h"
#include <iostream>
#include <termcolor.hpp>
#include <functional>
#include "ignorednicks.h"
#include <QThread>
AntiFlood::AntiFlood(QObject *parent) : QObject(parent)
{

}



void AntiFlood::receiveLine(QString& newLine, QTcpSocket* socket, IgnoredNicks *ignoredNicks_)
{

    ignoredNicks = ignoredNicks_;
    std::cout << termcolor::on_blue << "ANTIFLOOD receiveLine" << termcolor::reset<< std::endl;
    m_socket = socket;
    std::cout << "getNickAndRoom" << std::endl;
    std::string nick_ = extract.extractNick(newLine);
    room = extract.extractRoom(newLine);
    line = extract.extractText(newLine);
    if(nick_.find(".") != std::string::npos ||
        newLine.toStdString().find("PRIVMSG")== std::string::npos ||
        nick_.size() == 0 ||
        room.size() == 0 ||
        line.size() == 0 )
        return;

    nick = nick_; // so new speaker's nick wont replace the real flooder
    //clear same lines vectors if too much time passes
//    QTimer::singleShot(100000, [&](){
//        sameLines_v.erase(sameLines_v.begin(), sameLines_v.end());
//        linesFromChat_v.erase(linesFromChat_v.begin(), linesFromChat_v.end()); //its because sameLines_v is filled by linesFromChat_v

//        std::cout << termcolor::dark   << "CLEANING: " << sameLines_v.size() << termcolor::reset<< std::endl;
//    });

    std::cout << "antiflood nick: " << nick  << " room: " << room << " line: " << line << std::endl;

    //std::vector<std::pair<std::vector<std::string>, czas>> linesFromChat_v;
    czas currentTime = std::chrono::high_resolution_clock::now();
    //std::cout << "currentTime: " << std::chrono::system_clock::to_time_t(currentTime) << std::endl;

    linesFromChat_v.push_back( std::make_pair(std::vector<std::string>{nick, room, line}, currentTime));


    if(linesFromChat_v.size()<2) return; //no time to compare so return - its when game is fresh


    int counter = 3;  //in how many lines we count time   - this should be equal to the line in which the comment is : //usuba jak linii wiecej niz 3
    int timeBetweenTheLines = 3;

    //we go backward loop so we avoid old enties that are older than 3 seconds and which would cause that we d be in flood all the time:
    for (int i = linesFromChat_v.size()-1; i >=0; --i)
    {
        //if same nick and room:
        if(linesFromChat_v[i].first[0] == nick && linesFromChat_v[i].first[1] == room){ //if current nick and room == in long room and nick = if it finds matches (cause we only want time of the 3rd oldest entry)
            counter--;  //found current nick and line in history vector
            sameLines_v.push_back(linesFromChat_v[i].first[2]); //check if user didnt wrote the same lines
            std::cout << termcolor::red << "sameLines_v.size(): " << sameLines_v.size() << termcolor::reset<< std::endl;
        }


        //if same nick and room within 3 last lines of the same nick and room:
        if(sameLines_v.size()>= 3) //dont want the vector to store more than 3 entries:
            sameLines_v.erase(sameLines_v.begin(), sameLines_v.end()-3); //usuwa jak linii wiecej niz 3

        if(sameLines_v.size()== 3 && linesFromChat_v[i].first[0] == nick && linesFromChat_v[i].first[1] == room  &&  counter == 0){
            //========= for check if All elements are equal each other in sameLines_v - meaning if user wrote more than one:
            std::cout << termcolor::red << "###########################" << termcolor::reset<< std::endl;
            std::cout << "check if not all the same:" << std::endl;
            std::cout << "counter: " << counter << std::endl;
            std::cout << "sameLines_v.size(): " << sameLines_v.size() << std::endl;

            std::cout << termcolor::red << "###########################" << termcolor::reset<< std::endl;

            //bool is_flooding = false;

            if(std::adjacent_find(sameLines_v.begin(), sameLines_v.end(), std::not_equal_to<std::string>()) == sameLines_v.end() )
            {
                std::cout << termcolor::on_yellow << "same #############################################" << termcolor::reset<< std::endl;

                for (auto i : sameLines_v) {
                    std::cout << termcolor::red << "i2: " << i << termcolor::reset<< std::endl;
                }
                sendMessages(nick, room);
                ignoredNicks->addToIgnore(nick);

                sameLines_v.erase(sameLines_v.begin());
                return;
            }
            else
            {
                //remove first line even if no match
                sameLines_v.erase(sameLines_v.begin());
            }

            //===================== now for timer:

            std::cout << termcolor::red << "corresponding nicks: " << linesFromChat_v[i].first[0] << " and " << nick << "corresponding rooms: " << linesFromChat_v[i].first[1] << " and " << room << termcolor::reset<< std::endl;
            std::chrono::duration<float> duration0 = linesFromChat_v[i].second - currentTime;

            if(duration0.count() < -3){
                std::cout << "linesFromChat_v.size(): " << linesFromChat_v.size() << std::endl;
                std::cout << termcolor::green << " duration0.count() > timeBetweenTheLines "  << duration0.count() << termcolor::reset<< std::endl;
                linesFromChat_v.erase(linesFromChat_v.begin()+0); // remove olders lines which was the start of flood count
                std::cout << "linesFromChat_v.size(): " << linesFromChat_v.size() << std::endl;
                return;
            } else if(duration0.count()< -10 && sameLines_v.size()!= 0){ ///added sameLines_v
                //for same lines:

                sameLines_v.erase(sameLines_v.begin(), sameLines_v.end());
                linesFromChat_v.erase(linesFromChat_v.begin(), linesFromChat_v.end()); //this must be cleaned to cause it fills in loop sameLines_v at line 52
                std::cout << "CLEANING" << std::endl;
            }

            std::cout << termcolor::blue << " duration (time difference between 1st entry and newest): " << duration0.count() << termcolor::reset<< std::endl;

            //if user wrote i.e 3 or more lines and the time difference between the oldest and the newest is less than 3 seconds then it's a flooder:
            if( (duration0.count()) < timeBetweenTheLines ){ //3
                std::cout << termcolor::on_cyan << "FLOODER" << termcolor::reset<< std::endl;
                sendMessages(nick, room);
            }
        }///if same nick and room:
    }
}




void AntiFlood::sendMessages(std::string nick, std::string room)
{
    std::cout << termcolor::red << "sendMessages" << termcolor::reset<< std::endl;
    std::cout << "sendMessages" << std::endl;
    //nick = ":" + nick+ "!";
    //room = room + " :";

    std::string lineToPost = "KICK " +room + " " + nick + " 13przestań floodować! masz Bana na 10 sek"+ "\r\n";
    std::cout << "Line is: " + lineToPost<< "\n";
    //std::string lineToPost2 = "KICK " + room + " " + nick +  " 13przestań floodować łosiu! masz Bana na 10 sek"+ "\r\n";
    sendIT(lineToPost);


    //for capitalism:
    std::vector<std::string> flooder_v = {nick, room};
///kara finansowa
    //emit floodingUser(flooder_v);

    //QTimer::singleShot(5000, this, SLOT(unBan()));
    // Z założenia std::function to obiekt, który reprezentuje cokolwiek, co jest callable. Zamiast void(*func)() użyj std::function<void()>.



    auto lam = [this, room, nick](){
        QThread::msleep(15000);
        std::cout << termcolor::yellow << " lambda" << termcolor::reset<< std::endl;
        std::cout << termcolor::on_magenta << "room: " << room << " in antiflood lambda" << termcolor::reset<< std::endl;
        std::cout << termcolor::on_magenta << "nick: " << nick << " in antiflood lambda" << termcolor::reset<< std::endl;
        std::string lineToPost = "PRIVMSG " + room + " " + nick + " ok ban ściągniety ale nie flooduj więcej Łosiu!"+ "\r\n";
        //lineToPost = "KICK " + roomToPost +" "+ nickToPost + " przestań floodować łosiu!"+ "\r\n";
        ignoredNicks->Unignore(nick);
        std::cout << "UNIGNORE lam" << std::endl;
         //sendIT(lineToPost); //causes runtime error, cause inter-thread communication must be by signa/slot
        emit unbanInfoPost(QByteArray::fromStdString(lineToPost));    //this is instead of:  sendIT(lineToPost);, which was causing runtime error

    };

    //threadpool.globalInstance()->start([&, this] () mutable { AntiFlood::unBan();   });
//TODO //    threadpool.globalInstance()->start(lam); had to coment out lol


    flooder_v.clear();
}


void AntiFlood::sendIT(std::string line )
{
    lineToPost=line;
    const char* send = lineToPost.c_str();
    m_socket->write(send);
    m_socket->flush();
    lineToPost.clear(); //clear before next post
    m_socket->flush();
    m_socket->reset();

}




