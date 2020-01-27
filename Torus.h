#ifndef TORUS_H
#define TORUS_H

#include "Command.h"
#include "STLParser.h"


class Torus : public Command
{    
public:
    Torus(){}
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string> &args) override;

protected:
    Vec center;
    Vec axis;
    double bigRadius;
    double smallRadius;
    std::string filename;

    TriangleSoup soup;
    STLParser parser;
    std::vector<Vec> firstCircleStorage;
    std::vector<Vec> secondCircleStorage;
    std::vector<Vec> *circle;
    //std::vector<Vec> *firstCircle;
    //std::vector<Vec> *secondCircle;

    const std::string COMMAND_NAME = "Torus";
    const int QUALITY = 30;
    const Vec defNormal{0, 0, 0};
    const double epsi = 0.00001; // for comparing doubles

    Vec parseCoord(std::string  coords) const;
    void init(const Vec&, const Vec&, double, double, const std::string&);
    int checkInput(const std::map<std::string, std::string> &args) const;
    void createSoup();
    void findPlaneVectors(Vec&, Vec&) const;
    void createCircle(const Vec&);
    void circlesBinding(const std::vector<Vec>*, const std::vector<Vec>*);
};

#endif // TORUS_H
