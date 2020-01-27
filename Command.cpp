#include "Command.h"

// simple string split
std::vector<std::string> Command::split(std::string str) const {
    std::vector<std::string> result;
    std::string tempStr;
    str.push_back(' ');
    bool isWord = false;
    for (std::string::iterator strIt = str.begin(); strIt != str.end(); strIt++)
    {
        if (*strIt != ' ') {
            tempStr += *strIt;
            if (!isWord)
                isWord = true;
        }
        else if (isWord) {
            result.push_back(tempStr);
            tempStr.clear();
            isWord = false;
        }
    }

    return result;
}
