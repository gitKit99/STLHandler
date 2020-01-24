#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <memory>

#include "Command.h"


class Parser
{
public:
    Parser();
    int tryToParse(std::vector<std::string>, std::vector<std::unique_ptr<Command>> const&);
    std::map<std::string, std::string> getAttrDictionary();

private:
    std::vector<std::string> userCommand;
    std::vector<std::string>::iterator vectIt;
    void clean();
    void createMap();
    std::map<std::string, std::string> comMap;
};

#endif // PARSER_H
