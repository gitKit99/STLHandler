#include "Parser.h"

Parser::Parser()
{

}

int Parser::tryToParse(std::vector<std::string> userVector,
    std::vector<std::unique_ptr<Command>> const& comVector)
{
    this->userCommand.clear();
    this->userCommand = userVector;

    clean();
    createMap();
    return 0;
}

void Parser::clean()
{
    bool isCoordinates = false;
    for (vectIt = userCommand.begin(); vectIt < userCommand.end(); vectIt++)
    {
        if (*vectIt == "=" || (*vectIt == "," && !isCoordinates))
            userCommand.erase(vectIt--);
        else {
            if (((*vectIt)[0] == '(') && !isCoordinates)
            {
                isCoordinates = true;
            }
            else {
                if (isCoordinates)
                {
                    *(vectIt - 1) += *vectIt;
                    vectIt = userCommand.erase(vectIt);
                    vectIt--;
                }
                else if ((*vectIt).back() == ',')
                    (*vectIt).pop_back();
            }
            if (isCoordinates)
            {
                for (char c : *vectIt)
                {
                    if (c == ')') {
                        isCoordinates = false;
                        if ((*vectIt).back() == ',')
                            (*vectIt).pop_back();
                        break;
                    }
                }
            }
        }
    }
}

void Parser::createMap()
{
    for (vectIt = userCommand.begin(); vectIt < userCommand.end() - 1; vectIt += 2)
    {
        comMap[*vectIt] = *(vectIt + 1);
    }
}

std::map<std::string, std::string> Parser::getAttrDictionary()
{
    return this->comMap;
}
