#ifndef REMOVEDUPLICATES_H
#define REMOVEDUPLICATES_H
#include "replacer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>


class RemoveDuplicates
{
public:
    RemoveDuplicates();
    void removeDupl2(std::vector<std::string> &bufforVec);
    void replace_WordEnds2();
    void load_file_text2();
    void saveToFile2(std::vector<std::string> &bufforVec);
    void writeToFile2(std::ofstream &fileOut, std::string &buffor);
    std::string change_text2(std::vector< std::map<std::string, std::string> > &dictionary,
                            std::string &text);
    void remove2();
private:
    std::vector< std::map<std::string, std::string> > dictionary;
     std::vector<std::string> bufforVec;


};

#endif // REMOVEDUPLICATES_H
