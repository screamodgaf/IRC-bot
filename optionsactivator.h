#ifndef OPTIONSACTIVATOR_H
#define OPTIONSACTIVATOR_H
#include "replacer.h"
#include "removeduplicates.h"
#include <iostream>
#include <fstream>
#include <map>

class OptionsActivator
{
public:
    OptionsActivator();
    void replace_WordEnds();
    void remove_duplicates();

private:
    Replacer *replacer;
    RemoveDuplicates *removeDupli;
};

#endif // OPTIONSACTIVATOR_H
