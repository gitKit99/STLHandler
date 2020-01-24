#ifndef SPHERE_H
#define SPHERE_H

#include "Command.h"
#include "STLParser.h"


class Sphere : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    const std::string COMMAND_NAME = "Sphere";
    // ïàðàìåòðè ñôåðè
    double R;
    Vec origin;
    std::string outFile;

    void init(double R, Vec origin, std::string filename);
    Vec parseCoord(std::string coords);
    void calcVertices();
    void createSoup();
    int checkInput(const std::map<std::string, std::string>&);

    double map(const double& var, const double& begin, const double& end,
        const double& targetBegin, const double& targetEnd);

    const Vec defNormal = { 0.0, 0.0, 0.0 };
    Vertex** mesh;
    TriangleSoup soup;
    int tot = 20; // ê³ëüê³ñòü ñëàéñ³â òà ê³ëüê³ñòü ñòåê³â
    int pointsCount; // ê³ëüê³ñòü òî÷îê â ìåø³
    STLParser parser;
};

#endif // SPHERE_H
