#include "Application.h"

// add command pointer to command list (vector)
void Application::registerCommand(std::unique_ptr<Command> command) {
    this->comVector.push_back(std::move(command));
}

int Application::execute(int argc, char *argv[]) {
    // convert char array to vector container
    for (int i = 1; i < argc; i++) {    // 0 argument is program's path
        attrVector.push_back(argv[i]);
    }
    std::string commandName = attrVector[0];
    std::vector<std::unique_ptr<Command>>::iterator comIt = comVector.begin();
    while ((**comIt).getName() != commandName && comIt != comVector.end())
        comIt++;
    if (comIt == comVector.end())   // if command not found
        return 3;
    attrVector.erase(attrVector.begin());

    // trying to convert into map dictionary
    /*int canParsed = parser.tryToParse(attrVector, comVector);
    if (canParsed == 0)
        this->attrMap = parser.getAttrDictionary();
    else {
        std::cout << "ERROR " << canParsed << std::endl;
    }*/

    this->attrMap = parser.parse(attrVector);

    for (std::pair<std::string, std::string> pair : this->attrMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return (**comIt).execute(this->attrMap);
}
