#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>

#include "Parser.h"
#include "Command.h"


class Application
{
public:
    Application()   {};
    void registerCommand(std::unique_ptr<Command> command);
    int execute(int argc, char *argv[]);
private:
    std::vector<std::unique_ptr<Command>> comVector;
    std::vector<std::string> attrVector;
    Parser parser;
    std::map<std::string, std::string> attrMap;
};

#endif // APPLICATION_H
