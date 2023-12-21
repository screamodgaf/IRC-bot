#include "removedoublenicks.h"
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
/**
 *
 * first put line into cleanFileNames() then
 * the result to separateWords(line);
 * then both into firstWordFromSepwords(sepwords_v, trummeredNamesOfFiles);
 * then connect the separate words in vector into one string line by using connectWordsInVector()
 * example:
 *     std::vector<QString> trummeredNamesOfFiles =  cleanFileNames();
 *     std::vector<QString> sepwords_v = separateWords(line);
 *     std::vector<QString> vectorOFWordsWithout1_v = firstWordFromSepwords(sepwords_v, trummeredNamesOfFiles);
 *     std::string cleanedLine = connectWordsInVector(vectorOFWordsWithout1_v);
 *     std::cout << cleanedLine << std::endl;
 *
 *****************************************************************************************************/

RemoveDoubleNicks::RemoveDoubleNicks()
{

}


std::vector<std::string> RemoveDoubleNicks::cleanFileNames()
{
    std::vector<QString> vec;
    std::vector<QString> forbiddenSigns = {"", ".",".." , "BOT.txt"};

    QDirIterator it("config/", QDirIterator::Subdirectories);
    std::vector<std::string> trimmeredNames;

    while (it.hasNext())
    {
        //remove forbidenSigns and ".txt" and put into vector: trimmeredNames:
        if(std::find(forbiddenSigns.begin(), forbiddenSigns.end(), it.fileName())== forbiddenSigns.end())
        {
            QString temp = (it.fileName().remove(it.fileName().size()-4 , 4));
            trimmeredNames.push_back(temp.toStdString());
        }
        it.next();
    }

    return trimmeredNames;
}

std::vector<std::string> RemoveDoubleNicks::separateWords(std::string str)
{
    std::cout << "str: " << str << std::endl;
    std::string word = "";
    std::vector<std::string> sepwords_v;
    std::istringstream ss(str);

 // Traverse through all words
    do {
         std::string word;
         ss >> word;


       std::cout << word << std::endl;
       sepwords_v.push_back(word);
       // While there is more to read
       } while (ss);

    // now we have vector - sepwords_v of separate words

    //for (auto i : sepwords_v) {  std::cout << "po separowaniu: " <<  i << std::endl;}

    firstWord = sepwords_v[0]; //get first word in a sentence
    std::cout << "firstWord: "  << firstWord << std::endl;

    if(firstWord.size()>=3)
        firstWordTrimmered = firstWord.erase(firstWord.size()-1, firstWord.size()); //remove last letter of the first world (possible nicks) to get rid of "Emo:" instead only of "Emo"
    else
        firstWordTrimmered = firstWord;
    std::cout << "firstWordTrimmered: " << firstWordTrimmered << std::endl;




    return sepwords_v;
}

std::vector<std::string> RemoveDoubleNicks::firstWordFromSepwords (std::vector<std::string>& sepwords_v, std::vector<std::string>& listOfFileNames_v)
{
   // for (auto i : sepwords_v) {  std::cout << "before skonczone " << i << std::endl ; }

    //if first word in line is present in vector of nicks - means it is a nick, thus the word is nick so must be removed
    size_t posOfFind;
    for (int i = 0; i < listOfFileNames_v.size(); ++i)
    {
        if((posOfFind = listOfFileNames_v[i].find(firstWordTrimmered))!= std::string::npos && posOfFind == 0) //so firstWordTrimmered must be at the beginning of nick to get erased, not within nick, cause posOfFind == 0 - which means beginning
        {
            sepwords_v.erase(sepwords_v.begin());
            break;
        }
    }
    for (auto i : sepwords_v) {  std::cout << "skonczone " << i << "size: " << sepwords_v.size(); }
    return sepwords_v;
}

std::string RemoveDoubleNicks::connectWordsInVector(std::vector<std::string> &vectorOFWordsWithout1_v)
{
    std::string temp = "";
    for (int i = 0; i < vectorOFWordsWithout1_v.size(); ++i) {
        if(temp.size()!=0)
            temp += ' ';
        temp += vectorOFWordsWithout1_v[i];
    }
    std::cout << "temp: " << temp  <<"\n\n\n\n" << std::endl;
    return temp;
}

//main function:
std::string RemoveDoubleNicks::removeNicksFromBottxt(std::string& line, std::vector<std::string>& nicks_v) //remove doubled nicks in a line
{


    std::vector<std::string> sepwords_v = separateWords(line);
    std::vector<std::string> vectorOFWordsWithout1_v = firstWordFromSepwords(sepwords_v, nicks_v);
    std::string cleanedLine = connectWordsInVector(vectorOFWordsWithout1_v);


    return cleanedLine;
}
