#ifndef REPLACER_H
#define REPLACER_H
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>

class Replacer
{
public:
    Replacer();

    void replace_Ends();
    void make_dictionary();
    void load_fileText();
    void writeToFile(std::ofstream &fileOut, std::string &buffor);

private:
    std::string change_text(std::vector< std::map<std::string, std::string> > &dictionary,
                            std::string &text);
    std::vector< std::map<std::string, std::string> > dictionary;
};

#endif // REPLACER_H
