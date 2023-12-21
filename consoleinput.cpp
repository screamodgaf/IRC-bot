#include "consoleinput.h"
//#include "removeduplicates.h"
#include <iostream>
#include "colourmanipulation.h"
#include "settings.h"

void determineOption(std::string &input);
ConsoleInput::ConsoleInput( QObject *parent) : QObject(parent)
{

}

void ConsoleInput::startConsole(QThread *thread_)
{
    std::cout << "ConsoleInput::startConsole" << std::endl;
    thread = thread_;
    optionsActivator = new OptionsActivator;
    connect(thread, SIGNAL(started()), this, SLOT(run()));
}

bool ConsoleInput::weatherState()
{
    return weather_is;
}

bool ConsoleInput::recordTalkState()
{
    return recordTalk_is;
}

bool ConsoleInput::newsState()
{
    return news_is;
}

bool ConsoleInput::saveNowState()
{
    return saveNow_is;
}

void ConsoleInput::set_saveNow_is_toFalse()
{
    saveNow_is = false;
}

bool ConsoleInput::botSpeechState()
{
    return botSpeech_is;
}

bool ConsoleInput::englishPostsState()
{
    return english_is;
}

bool ConsoleInput::greetingsState()
{
    return greetings_is;
}

bool ConsoleInput::capitalismState()
{
    return capitalismGame_is;
}


void ConsoleInput::run()
{
    std::cout << "!ConsoleInput::run()" << "\n";

    while(1){
    std::string input;


    std::getline(std::cin, input);
    std::cout << "=========================" << "\n";
    std::cout << input << "\n";
    determineOption(input);

    if(input[0] != '/' || input[0] != '\\')
        postInput(input);


    input.clear();
    }
}

void ConsoleInput::runFromIRC(std::string &data)
{
    determineOption(data);
    std::cout << "#runFromIRC" << "\n";

    if(data[0] == '/')
    {
        postRawInput(data);
        std::cout << "#runFromIRC: postInput(data): " << data << "\n";
    }
    else if(data[0] != '/')
        postInput(data);
}


void ConsoleInput::determineOption(std::string input)
{
    if(input[0] == '/' || input[0] == '\\')
    {
        input.erase(0,1);
        std::map<std::string,int>mapa;
        mapa["replace words"]  = 1;
        mapa["remove duplicates"]  = 2;
        mapa["weather off"]  = 3;
        mapa["weather on"]  = 4;
        mapa["news off"]  = 5;
        mapa["news on"]  = 6;
        mapa["record off"]  = 7;
        mapa["record on"]  = 8;
        mapa["save"]  = 9;
        mapa["bot off"]  = 10;
        mapa["bot on"]  = 11;
        mapa["say"]  = 12;
        mapa["english on"]  = 13;
        mapa["english off"]  = 14;
        mapa["greetings on"]  = 15;
        mapa["greetings off"]  = 16;
        mapa["cap on"]  = 17;
        mapa["cap off"]  = 18;
        int option;
        /*std::map<std::string,int>::iterator it;
        for (it = mapa.begin(); it != mapa.end(); ++it)
        {
            if(it->first == input)
                option = mapa[input];
        }
//switch (option) */
        switch (mapa[input]) {
        case 1:
            optionsActivator->replace_WordEnds();
            return;
        case 2:
            optionsActivator->remove_duplicates();
            return;
        case 3:
            weather_is = false;
            std::cout << "weather_is = false" << "\n";
            return;
        case 4:
            weather_is = true;
            std::cout << "weather_is = true" << "\n";
            return;
        case 5:
            news_is = false;
            std::cout << "news_is = false" << "\n";
            return;
        case 6:
            news_is = true;
            std::cout << "news_is = true" << "\n";
            return;
        case 7:
            recordTalk_is = false;
            std::cout << "recordTalk_is = false" << "\n";
            return;
        case 8:
            recordTalk_is = true;
            std::cout << "recordTalk_is = true" << "\n";
            return;
        case 9:
            saveNow_is = true;
            std::cout << "saveNow_is = true" << "\n";
            emit saveSignal();
            return;
        case 10:
            botSpeech_is = false;
            std::cout << "botSpeech_is = false" << "\n";
            return;
        case 11:
            botSpeech_is = true;
            std::cout << "botSpeech_is = true" << "\n";
            return;
        case 12:
            emit sayNow();
            std::cout << "sayNow()" << "\n";
            return;
        case 13:
            english_is = true;
            std::cout << "english = true" << "\n";
            return;
        case 14:
            english_is = false;
            std::cout << "english = false" << "\n";
            return;
        case 15:
            greetings_is = true;
            std::cout << "greetings = true" << "\n";
            return;
        case 16:
            greetings_is = false;
            std::cout << "greetings = false" << "\n";
            return;
        case 17:
            capitalismGame_is = true;
            std::cout << "capitalism game = true" << "\n";
            return;
        case 18:
            capitalismGame_is = false;
            std::cout << "capitalism game = false" << "\n";
            return;
       default:
            postRawInput(input);
        }
    }


    //        mapa["english on"]  = 13;
    //mapa["english off"]  = 14;
    //postRawInput(input);
}

void ConsoleInput::postInput(std::string &input)
{
    if(input[0]=='/')
        return; //this prevents bot poting "/command" to channel
    std::string lineToPost;
    std::cout << "!lineToPost - input " << input << "\n";



    lineToPost = "PRIVMSG "+ Settings::getMyRoom().toStdString() +" :" + input  + "\r\n";
    emit mySignal(QByteArray::fromStdString(lineToPost)); // problem
}

void ConsoleInput::postRawInput(std::string &input)// for commands
{

    std::string lineToPost;

    lineToPost = input + "\r\n";

    //const char *send = lineToPost.c_str();


    emit mySignal(QByteArray::fromStdString(lineToPost));
}


