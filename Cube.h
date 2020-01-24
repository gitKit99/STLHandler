#ifndef CUBE_H
#define CUBE_H

#include "Command.h"
#include "STLParser.h"


class Cube : public Command
{
public:
    Cube() {};
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    const std::string COMMAND_NAME = "Cube";
    // ïàðàìåòðè êóáà
    std::string outFile;
    double L;
    Vec origin;

    void init(double L, Vec origin, std::string filename);
    Vec parseCoord(std::string  coords);
    void calcVertices();
    void createSoup();
    int checkInput(const std::map<std::string, std::string>& args);

    std::vector<Vertex> mesh;
    TriangleSoup soup;
    const Vec defNormal{ 0, 0, 0 };

    STLParser parser;
};

#endif // CUBE_H
