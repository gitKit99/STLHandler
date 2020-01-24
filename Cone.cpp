#include "Cone.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iterator>

const std::string &Cone::getName() const
{
    return this->COMMAND_NAME;
}

int Cone::execute(const std::map<std::string, std::string> &args)
{
    int argCheck = this->checkInput(args);

    if (argCheck != 0)
        return argCheck;

    // cone parameters initialization by arguments
    this->init(parseCoord(args.at("basecenter")),
               parseCoord(args.at("basenormal")),
               atof(args.at("radius").c_str()),
               atof(args.at("height").c_str()),
               args.at("filepath"));

    this->calcVertices();
    this->createSoup();

    std::cout << "Count of triangles " << this->soup.size() / 3 << std::endl;

    parser.write(this->soup, this->outFile);  // write in stl file

    return 0;
}

void Cone::init(Vec baseCenter, Vec baseNormal, double baseRadius, double height,
                std::string filename)
{
    this->base.center = baseCenter;
    this->base.radius = baseRadius;

    // given normal vector normalization (if necessary)
    this->base.normal = vectorNormalize(baseNormal);

    // find top vertex of cone
    // (normal points in opposite direction of cone's top)
    this->origin = Vec{this->base.center.x - this->base.normal.x * height,
                       this->base.center.y - this->base.normal.y * height,
                       this->base.center.z - this->base.normal.z * height};

    this->outFile = filename;
}

Vec Cone::parseCoord(std::string  coords)
{
    coords.erase(std::remove(coords.begin(), coords.end(), '('),
        coords.end());
    coords.erase(std::remove(coords.begin(), coords.end(), ')'),
        coords.end());
    std::replace(coords.begin(), coords.end(), ',', ' ');

    std::vector<std::string> coordVec = split(coords);

    return Vec{atof(coordVec[0].c_str()), atof(coordVec[1].c_str()),
                atof(coordVec[2].c_str())};
}

void Cone::calcVertices()
{
    // push top and base center of cone
    this->mesh.push_back(Vertex{this->origin, defNormal});
    this->mesh.push_back(Vertex{this->base.center, defNormal});

    // find two orthogonal vectors, lying on base plane,
    // for drawing base circle
    double shift = 5; // find one more point on plane using point shifting
    Vec somePlanePoint = this->base.center;
    somePlanePoint.x += shift;
    somePlanePoint = this->PointOnPlaneProject(somePlanePoint,
                                               this->base.center,
                                               this->base.normal);

    if ((fabs(somePlanePoint.x - this->base.center.x) < epsi)
        && (fabs(somePlanePoint.y - this->base.center.y) < epsi)
        && (fabs(somePlanePoint.z - this->base.center.z) < epsi)) {
        // we met the same point
        somePlanePoint = this->base.center;
        somePlanePoint.y += shift;
        somePlanePoint = this->PointOnPlaneProject(somePlanePoint,
                                                   this->base.center,
                                                   this->base.normal);
    }

    Vec vecOnPlane1 = Vec{somePlanePoint.x - this->base.center.x,
                          somePlanePoint.y - this->base.center.y,
                          somePlanePoint.z - this->base.center.z};
    vecOnPlane1 = vectorNormalize(vecOnPlane1);

    Vec vecOnPlane2 = vectorProduct(this->base.normal, vecOnPlane1);

    // push vertices of base ring
    double x, y, z;
    Vec vertex;
    int slicesCount = 60;

    for (int i = 0; i < slicesCount; i++) {
        double theta = this->map(i, 0, slicesCount, 0, 2 * M_PI);

        x = this->base.center.x
                + this->base.radius * cos(theta) * vecOnPlane1.x
                + this->base.radius * sin(theta) * vecOnPlane2.x;

        y = this->base.center.y
                + this->base.radius * cos(theta) * vecOnPlane1.y
                + this->base.radius * sin(theta) * vecOnPlane2.y;

        z = this->base.center.z
                + this->base.radius * cos(theta) * vecOnPlane1.z
                + this->base.radius * sin(theta) * vecOnPlane2.z;

        vertex = {x, y, z};

        this->mesh.push_back(Vertex{vertex, defNormal});
    }


}

void Cone::createSoup()
{
    Vertex baseCenter{this->base.center, defNormal},
            top{this->origin, defNormal};

    // add first vertex of ring to end of vector
    // for surface looping
    this->mesh.push_back(*(this->mesh.cbegin() + 2));

    // pass by ring vertices and bind them with top and circle center
    for (TriangleSoup::const_iterator it = this->mesh.cbegin() + 3;
         it != this->mesh.cend();
         it++) {
        this->soup.push_back(baseCenter);
        this->soup.push_back(*it);
        this->soup.push_back(*(it - 1));

        this->soup.push_back(top);
        this->soup.push_back(*(it - 1));
        this->soup.push_back(*it);
    }

    /*this->soup.push_back(baseCenter);
    this->soup.push_back(*(this->mesh.cbegin() + 2));
    this->soup.push_back(*(this->mesh.cend() - 1));

    this->soup.push_back(top);
    this->soup.push_back(*(this->mesh.cend() - 1));
    this->soup.push_back(*(this->mesh.cbegin() + 2));*/
}

int Cone::checkInput(const std::map<std::string, std::string> &input)
{
    // check whether all arguments are present
    if (input.find("basecenter") == input.end() ||
        input.find("basenormal") == input.end() ||
        input.find("radius") == input.end() ||
        input.find("height") == input.end() ||
        input.find("filepath") == input.end())
        return 3;

    // check whether lengths are integral
    if (atof(input.at("radius").c_str()) <= 0
        || atof(input.at("height").c_str()) <= 0)
        return 1;

    return 0;
}

Vec Cone::PointOnPlaneProject(const Vec & point,
                              const Vec & pointOnPlane,
                              const Vec & planeNormal) const
{
    Vec v = planeNormal;
    Vec w{pointOnPlane.x - point.x, pointOnPlane.y - point.y, pointOnPlane.z - point.z};
    Vec intersectPoint{};
    double denom = dotProduct(planeNormal, v);
    if (fabs(denom) >= 0.00001)
    {
        double t = dotProduct(planeNormal, w) / denom;
        intersectPoint.x = point.x + v.x * t;
        intersectPoint.y = point.y + v.y * t;
        intersectPoint.z = point.z + v.z * t;
    }
    else std::cout << "Point of intersection has not found" << std::endl;
    return intersectPoint;
}

double Cone::dotProduct(const Vec & v1, const Vec & v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// convert value from one interval to another
double Cone::map(const double & var, const double & begin, const double & end,
    const double & targetBegin, const double & targetEnd) const
{
    double result = targetBegin + ((var - begin) * (targetEnd - targetBegin)) /
        (end - begin);
    return result;
}

double Cone::vectorLength(const Vec &vec)
{
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

Vec Cone::vectorNormalize(Vec v)
{
    double normalLength = vectorLength(v);
    if (normalLength > 0.00001
        && fabs(normalLength - 1.0) > 0.00001) {
        Vec normalized = Vec{v.x / normalLength,
                             v.y / normalLength,
                             v.z / normalLength};
        return normalized;
    }
    else {
        return v;
    }
}

Vec Cone::vectorProduct(const Vec &a, const Vec &b)
{
    return Vec{a.y * b.z - a.z * b.y,
               a.z * b.x - a.x * b.z,
               a.x * b.y - a.y * b.x};
}

