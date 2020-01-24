#ifndef CONE_H
#define CONE_H

#include <Command.h>
#include <STLParser.h>

struct Circle3D
{
    Vec center;
    Vec normal;
    double radius;
};


class Cone : public Command
{
public:
    Cone(){};
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string> &args) override;

protected:
    const std::string COMMAND_NAME = "Cone";
    // cone building parameters
    std::string outFile;
    Vec origin;
    Circle3D base;

    void init(Vec baseCenter, Vec baseNormal, double radius, double height, std::string filename);
    Vec parseCoord(std::string coords);
    void calcVertices();
    virtual void createSoup();
    int checkInput(const std::map<std::string, std::string>& args);

    Vec PointOnPlaneProject(const Vec & point, const Vec & pointOnPlane, const Vec & planeNormal) const;
    double map(const double & var, const double & begin, const double & end,
        const double & targetBegin, const double & targetEnd) const;

    static double vectorLength(const Vec& v);
    static Vec vectorNormalize(Vec v);
    static double dotProduct(const Vec & v1, const Vec & v2);
    static Vec vectorProduct(const Vec& a, const Vec& b);

    TriangleSoup mesh;
    TriangleSoup soup;
    const Vec defNormal{0, 0, 0};
    const double epsi = 0.00001;

    STLParser parser;
};

#endif // CONE_H
