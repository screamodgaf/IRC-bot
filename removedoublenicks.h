#ifndef REMOVEDOUBLENICKS_H
#define REMOVEDOUBLENICKS_H

#include <QObject>

class RemoveDoubleNicks
{
public:
    RemoveDoubleNicks();
    std::vector<std::string> cleanFileNames();
    std::string removeNicksFromBottxt(std::string& line_, std::vector<std::string>& nicks_v);
private:
    std::string firstWord;
    std::string firstWordTrimmered;
    std::vector<std::string> separateWords(std::string str);
    std::vector<std::string> firstWordFromSepwords(std::vector<std::string> &sepwords_v, std::vector<std::string> &listOfFileNames_v);
    std::string connectWordsInVector(std::vector<std::string> &vectorOFWordsWithout1_v);

};

#endif // REMOVEDOUBLENICKS_H
