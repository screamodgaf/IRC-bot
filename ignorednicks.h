#ifndef IGNOREDNICKS_H
#define IGNOREDNICKS_H
#include <vector>
#include <QString>
#include "extract_nickandroom.h"
class IgnoredNicks
{
public:
    IgnoredNicks();
    bool checkForIgnoredNicks(QString data);
    void fill_ignoredNicks_v();
    void addToIgnore(std::string nick);
    void Unignore(std::string nick);
private:

    std::vector<std::string> ignoredNicks_v;
    Extract_NickAndRoom e;
};

#endif // IGNOREDNICKS_H
