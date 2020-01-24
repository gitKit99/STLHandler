#ifndef COMMAND_H
#define COMMAND_H

#include <map>
#include <string>
#include <vector>


class Command
{
public:
    virtual const std::string& getName() const = 0;

    virtual int execute(const std::map<std::string, std::string>& args) = 0;
protected:
    virtual std::vector<std::string> split(std::string str); // simple string split
};

#endif // COMMAND_H
