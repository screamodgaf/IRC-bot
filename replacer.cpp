#include "replacer.h"

void replace_Ends();
void make_dictionary();
void load_fileText() ;
std::string change_text(std::vector< std::map<std::string, std::string> > &dictionary2,
                        std::string &text);
void writeToFile(std::ofstream &fileOut, std::string &buffor);

Replacer::Replacer()
{

}

void Replacer::replace_Ends() //starts
{
    std::cout << "Starting replacing..." << "\n";
    make_dictionary();
    load_fileText();
}

void Replacer::make_dictionary()
{
    std::map<std::string, std::string> map1;
    map1.insert(std::pair<std::string, std::string>("łem", "łam"));

    std::map<std::string, std::string> map2;
    map2.insert(std::pair<std::string, std::string>("lem", "lam"));

    std::map<std::string, std::string> map3;
    map3.insert(std::pair<std::string, std::string>("lem", "lam"));

    dictionary.push_back(map1);
    dictionary.push_back(map2);
    //dictionary.push_back(map3);
}

void Replacer::load_fileText()
{
    std::ifstream fileIn("config/BOT.txt");

    std::vector<std::string> bufforVec;
    std::string temp;
    size_t index =0;
    while(std::getline(fileIn, temp))
    {
        bufforVec.push_back(temp);
        bufforVec[index] = change_text(dictionary, bufforVec[index]);
        index++;
    }
    fileIn.close();

    std::ofstream fileOut("config/BOT.txt"); //make replace
    for (auto i = 0; i < bufforVec.size(); ++i)
    {
        writeToFile(fileOut, bufforVec[i]);
        std::cout << "writting to file: " << bufforVec[i] << "\n";
    }


    fileOut.close();
}

void Replacer::writeToFile(std::ofstream &fileOut, std::string &buffor)
{
    fileOut<<buffor + "\n";
}



std::string Replacer::change_text(std::vector< std::map<std::string, std::string> > &dictionary,
                        std::string &text)
{
    size_t pos;
    for(auto i =0; i < dictionary.size(); i++)
    {
        if((pos=text.find( dictionary[i].begin()->first )) !=std::string::npos)
        {
            //std::cout << "found match: " << pos << "\n";
            text = text.replace(pos, (dictionary[i].begin()->first).size(),
                         dictionary[i].begin()->second);
        }
    }
    return text;
}
