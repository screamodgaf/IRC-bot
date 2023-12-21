#ifndef RECORDTALKS_H
#define RECORDTALKS_H
#include "extract_nickandroom.h"
#include "consoleinput.h" //for setting saveNow_on to = again
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <iostream>
#include <fstream>
#include <vector>
#include "Capitalism/capitalismstorage.h"
#include <QDir>
class RecordTalks: public QObject
{
    Q_OBJECT

public:
    RecordTalks(QObject *parent = nullptr, CapitalismStorage *capitalismStorage_ = nullptr);
    ~RecordTalks();
    void run(QString data);
    void run();
    void triggerSaveNow();
    std::string getDatabaseUpdate();

signals:
    void textUpdateSignal(std::string subText);
private:
    void extractText(QString &data);
    void extractNick(std::string &text);
    void extractRoom(std::string &text);
    void pushToVector(std::string nick, std::string &subText);
    void saveToFile(std::vector<std::pair<std::string, std::string> > &allTexts, std::string nickFileName);
    void match_nicks_and_allTexts(std::vector< std::pair<std::string, std::string>> allTexts);
    void createNickFiles(std::vector<std::string> &allTexts);
    void removeDuplicatedNicks();
    void closeNickFiles();
    std::string removeDigitsAndSpecials(std::string& nick);
    //capitalism:

private:

    std::string text;
    std::string textUpdate;
    std::string nick;
    std::string room;
    std::string subText;
    QDir directory;
    std::vector< std::pair<std::string, std::string> > allTexts; //saved nicks and texts
    std::ofstream file;
    std::vector <std::string> nicks_v;
    std::pair<std::string, std::fstream> nickAndFile_pair;
    std::vector< std::pair<std::string, std::fstream> > nickAndFile_pair_vec;
    bool save;
    ConsoleInput console;

    //capitalism
    CapitalismStorage* capitalismStorage;

};

#endif // RECORDTALKS_H


