#include "removeduplicates.h"
void load_file_text2();
void writeToFile2(std::ofstream &fileOut, std::string &buffor);
void saveToFile2(std::vector<std::string> &bufforVec);
void removeDupl2(std::vector<std::string> &bufforVec);

RemoveDuplicates::RemoveDuplicates()
{

}

void RemoveDuplicates::remove2()
{
    std::cout << "Starting removing duplicates..." << "\n";
    load_file_text2();
    saveToFile2(bufforVec);
}


void RemoveDuplicates::load_file_text2()
{
    std::ifstream fileIn("config/BOT.txt");

    std::string temp;
    size_t index =0;
    while(std::getline(fileIn, temp))
    {
        bufforVec.push_back(temp);
    }
    fileIn.close();
    removeDupl2(bufforVec);
}

void RemoveDuplicates::removeDupl2(std::vector<std::string> &bufforVec)
{
    for (auto i=0; i< bufforVec.size(); i++)
    {
        for (auto j=i+1; j< bufforVec.size(); j++)
        {
            if(bufforVec[i]== bufforVec[j])
            {
                bufforVec.erase(bufforVec.begin()+j);
                --j;
            }
        }
    }
}

void RemoveDuplicates::saveToFile2(std::vector<std::string> &bufforVec)
{
    std::ofstream fileOut("config/BOT.txt"); //make replace
    for (auto i = 0; i < bufforVec.size(); ++i)
    {
        writeToFile2(fileOut, bufforVec[i]);
        std::cout << "writting to file: " << bufforVec[i] << "\n";
    }

    fileOut.close();
}





void RemoveDuplicates::writeToFile2(std::ofstream &fileOut, std::string &buffor)
{
    fileOut<<buffor + "\n";
}



std::string RemoveDuplicates::change_text2(std::vector< std::map<std::string, std::string> > &dictionary,
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


