#include "optionsactivator.h"


OptionsActivator::OptionsActivator()
{

}

void OptionsActivator::replace_WordEnds()
{
    replacer = new Replacer;
    replacer->replace_Ends();
}

void OptionsActivator::remove_duplicates()
{
    removeDupli = new RemoveDuplicates;
    removeDupli->remove2();
}
