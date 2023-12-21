#ifndef LOADLOGS_H
#define LOADLOGS_H
#include <fstream>
#include <vector>
#include <unordered_set>
#include <QString>
#include "Capitalism/capital.h"
#include <QDir>

class RemoveDoubleNicks;

class LoadLogs
{
public:
    LoadLogs();
    ~LoadLogs();
    void load();
    void loadEnglishLogs();
    void loadtexts(); //hejt
    void loadGreetings();
    void loadListOfCapitalists();
    std::vector<std::string> &databaseGetter();
    std::vector<std::string> englishDatabaseGetter();
    std::unordered_set<std::string> *textsGetter();
    std::vector<std::string>& getNicksFromLogs();
    std::vector<std::string>& getGreetingVector();
    std::vector<std::string>& get_listOfCapitalists();
private:
    std::fstream file;
    std::vector<std::string> nicks_v;
    std::vector<std::string> englishDatabase;
    std::vector<std::string> database;
    std::vector<std::string> greetings_v;
    std::unordered_set<std::string> uset_Texts;
    std::vector<std::string>* listOfCapitalists_v;

    QDir directory;

};

#endif // LOADLOGS_H
