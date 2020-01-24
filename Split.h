#ifndef SPLIT_H
#define SPLIT_H

#include "Command.h"
#include "STLParser.h"

class Split : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    // ïàðàìåòðè ñïë³òà
    std::string input;
    Vec origin;
    Vec direction;
    std::string output1;
    std::string output2;

    int checkInput(const std::map<std::string, std::string>&);
    void init(std::string input, Vec origin, Vec direction, std::string output1,
        std::string output2);
    Vec parseCoord(std::string  coords);

    int splitSoup();
    double planeEquation(const Vec&);
    Vec linePlaneIntersect(const Vec& p0, const Vec&p1);
    double dotProduct(const Vec& v1, const Vec& v2);
    void createSurfaceSoup();
    void pushBackVector(TriangleSoup &dest, const TriangleSoup &vector);
    double vectorLength(const Vec&);
    double vectorsAngle(const Vec&, const Vec&);
    double distBtwnPoints(const Vec&, const Vec&);
    bool isContain(const TriangleSoup&, const Vec&);
    void sortBoundVertices();
    void triangSurface();
    void outputSurfPoints();
    //double detMatr3n(double a[3][3]);
    //bool isCounterClockwise(const Vec& v0, const Vec& v1, const Vec& v2);

    const std::string COMMAND_NAME = "Split";
    const Vec defNormal{ 0, 0, 0 };
    STLParser parser;
    TriangleSoup sourceSoup;
    TriangleSoup firstSoup;
    TriangleSoup secondSoup;

    TriangleSoup boundPoints;
    TriangleSoup surfaceSoup;
    TriangleSoup sortedBordPoints;
    std::multimap<double, Vertex> surfPoints;

    bool isDirectByPass;
};

#endif // SPLIT_H
