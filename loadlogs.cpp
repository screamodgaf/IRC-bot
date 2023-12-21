#include "loadlogs.h"
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>
#include <removedoublenicks.h>
#include <QDebug>
#include <QCoreApplication>
LoadLogs::LoadLogs()
{
    directory = QCoreApplication::applicationDirPath();
}

LoadLogs::~LoadLogs()
{
    std::cout << "LOADLOGS DESTRUCTOR\n\n\n\n\n\n\n\n" << "\n";
}

void LoadLogs::load()
{
    std::cout << "--A--LOADING DATABASE -" << "\n";

    QString relativePath = directory.absoluteFilePath("config/BOT.txt");
    qDebug() << "LoadLogs::relativePath: " << relativePath;
    file.open(relativePath.toStdString(), std::ios::in);
    std::string temp;
    database.reserve(700000);

    RemoveDoubleNicks r;
    nicks_v = r.cleanFileNames(); // get nicks from logs

    //std::cout << "NICKS_V SIZE: " << nicks_v.size() << std::endl << std::endl << std::endl << std::endl << std::endl << std::endl ;

    while (getline(file, temp))
    {
        database.push_back(temp);
    }
    file.close();


    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(database), std::end(database), rng); // 1 shuffle at the ebginning of program


    std::cout << "--B----LOADING DATABASE -" << "\n";
    std::cout << termcolor::green << "database.size(): " << database.size()  << termcolor::reset<< std::endl;
}

std::vector<std::string>& LoadLogs::getNicksFromLogs()
{
    std::cout << "RETURNING nicks v size: " << nicks_v.size() << std::endl;
    return nicks_v;
}

std::vector<std::string> &LoadLogs::getGreetingVector()
{
    return greetings_v;
}

std::vector<std::string>& LoadLogs::get_listOfCapitalists()
{
    return *listOfCapitalists_v;
}


std::vector<std::string>& LoadLogs::databaseGetter()
{
   return database;
}

void LoadLogs::loadEnglishLogs()
{
    std::cout << "CALLING ENGLISH.TXT\n\n\n" << std::endl;
    std::string temp;

    QString relativePath = directory.absoluteFilePath("config/english.txt");
    std::fstream file(relativePath.toStdString(), std::ios::in);
    englishDatabase.reserve(700);
    while(std::getline(file, temp))
    {
        englishDatabase.emplace_back(temp);
    }
    file.close();
    //srand(unsigned(time(nullptr)));

    //std::random_shuffle(englishDatabase.begin(), englishDatabase.end());
}

void LoadLogs::loadtexts() //hejt
{
    std::string temp;
    uset_Texts.reserve(200);
    QString relativePath = directory.absoluteFilePath("config/texts.txt");

    std::ifstream file(relativePath.toStdString(), std::ios::in );
    while(std::getline(file, temp))
        uset_Texts.insert(temp);

    file.close();
}

void LoadLogs::loadGreetings()
{
    std::cout << "loadGreetings()" <<  std::endl;
    std::string tempg;
    greetings_v.reserve(50);
    QString relativePath = directory.absoluteFilePath("config/greetings.txt");
    std::ifstream fileG(relativePath.toStdString());
    while(std::getline(fileG, tempg))
        greetings_v.push_back(tempg);

    std::cout << "greetings_v.size: " << greetings_v.size() << std::endl;
    fileG.close();
}


void LoadLogs::loadListOfCapitalists()
{
    std::cout << "loadListOfCapitalists()" <<  std::endl;
    std::string temp;
    listOfCapitalists_v = new std::vector<std::string>;
    listOfCapitalists_v->reserve(50);

    QString relativePath = directory.absoluteFilePath("config/capitalism.txt");
    std::cout << "LoadLogs::loadListOfCapitalists() relativePath: " << relativePath.toStdString() << "\n";
    std::ifstream file(relativePath.toStdString());

    while(std::getline(file, temp)){
        std::cout << "getline: " << temp << std::endl;
        listOfCapitalists_v->push_back(temp);
    }
    std::cout << "capitalism: temp_v.size: " <<  listOfCapitalists_v->size() << std::endl;
    file.close();
    std::cout << "loadListOfCapitalists() finished" << std::endl;


}




std::vector<std::string> LoadLogs::englishDatabaseGetter()
{
    return englishDatabase;
}

std::unordered_set<std::string> *LoadLogs::textsGetter()
{
    return &uset_Texts;
}

