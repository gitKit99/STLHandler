#include "Torus.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>

const std::string& Torus::getName() const
{
    return COMMAND_NAME;
}

int Torus::execute(const std::map<std::string, std::string> &args)
{
    int argCheck = this->checkInput(args);

    if (argCheck != 0)
        return argCheck;

    // torus parameters initialization by arguments
    this->init(parseCoord(args.at("center")),
               parseCoord(args.at("axis")),
               atof(args.at("R").c_str()),
               atof(args.at("r").c_str()),
               args.at("filepath"));

    this->createSoup();

    std::cout << "Count of triangles " << this->soup.size() / 3 << std::endl;

    parser.write(this->soup, this->filename);  // write in stl file


    return 0;
}

// point's coordinates parsing
Vec Torus::parseCoord(std::string  coords) const
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

void Torus::init(const Vec &center, const Vec &axis, double R, double r,
                 const std::string &filename)
{
    this->center        = center;
    this->axis          = axis;
    this->bigRadius     = R;
    this->smallRadius   = r;
    this->filename      = filename;

    parser.vectorNormalize(this->axis);

    // reserving space for points of one circle
    this->firstCircleStorage.reserve(static_cast<unsigned long>(QUALITY));
    this->secondCircleStorage.reserve(static_cast<unsigned long>(QUALITY));

    this->circle = &firstCircleStorage; // points where points will be written
}

int Torus::checkInput(const std::map<std::string, std::string> &input) const
{
    // check whether all arguments are present
    if (input.find("center") == input.end() ||
        input.find("axis") == input.end() ||
        input.find("R") == input.end() ||
        input.find("r") == input.end() ||
        input.find("filepath") == input.end())
        return 3;

    // check whether lengths are integral
    if (atof(input.at("R").c_str()) <= 0
        || atof(input.at("r").c_str()) <= 0
            // if two times radius of small circle is bigger than radius of big circle
        || atof(input.at("R").c_str()) < (2.0 * atof(input.at("r").c_str())))
        return 1;

    return 0;
}

// create triangulated mesh
void Torus::createSoup()
{
    // first, we will find points of big circle, that lyings on the given plane
    // for defining circle on a plane we need two orthogonal vectors,
    // that belong the plane
    Vec v1, v2;
    this->findPlaneVectors(v1, v2); // vectors' initialization

    // finding every small circle and push it to triangle soup
    double theta;
    Vec circleCenter;
    Vec start;
    for (int circle = 0; circle < QUALITY; circle++)
    {
        theta = parser.map(circle, 0, QUALITY, 0, 2 * M_PI);

        circleCenter.x = center.x + bigRadius * cos(theta) * v1.x
                        + bigRadius * sin(theta) * v2.x;
        circleCenter.y = center.y + bigRadius * cos(theta) * v1.y
                        + bigRadius * sin(theta) * v2.y;
        circleCenter.z = center.z + bigRadius * cos(theta) * v1.z
                        + bigRadius * sin(theta) * v2.z;

        // remember first circle's center for torus's closed surface
        if (circle == 0)
            start = circleCenter;

        // this function's responsible for small circle creation
        // and its adding to soup
        createCircle(circleCenter);
    }

    createCircle(start);
}

// this method should calculate two vectors, so we pass them as arguments
// by reference, and this method will initialize them with needed values
void Torus::findPlaneVectors(Vec &v1Out, Vec &v2Out) const
{
    // find two orthogonal vectors, lying on base plane,
    // for drawing big circle
    double shift = 5; // find one more point on plane using point shifting
    Vec somePlanePoint = this->center;
    somePlanePoint.x += shift;
    somePlanePoint = parser.PointOnPlaneProject(somePlanePoint,
                                               this->center,
                                               this->axis);

    if ((fabs(somePlanePoint.x - this->center.x) < epsi)
        && (fabs(somePlanePoint.y - this->center.y) < epsi)
        && (fabs(somePlanePoint.z - this->center.z) < epsi)) {
        // we met the same point
        somePlanePoint = center;
        somePlanePoint.y += shift;
        somePlanePoint = parser.PointOnPlaneProject(somePlanePoint,
                                                   this->center,
                                                   this->axis);
    }

    v1Out = Vec{this->center.x - somePlanePoint.x,
                this->center.y - somePlanePoint.y,
                this->center.z - somePlanePoint.z};

    parser.vectorNormalize(v1Out);

    v2Out = parser.crossProduct(v1Out, this->axis);
}

void Torus::createCircle(const Vec& center)
{
    static bool isCircleFirst = true;

    // we should define circle again, but a small one
    // a plane, which needed circle belongs, defines by point (circle center),
    // and two vectors, that belong it (first it's axis and second is a vector
    // from big circle center to small one (which this method get)
    Vec v1{ this->center.x - center.x,
                this->center.y - center.y,
                this->center.z - center.z   };
    parser.vectorNormalize(v1);

    Vec v2 = this->axis;

    this->circle->clear();

    double theta;
    Vec circlePoint;
    for (int i = 0; i < QUALITY; i++)
    {
        theta = parser.map(i, 0.0, QUALITY, 0.0, 2 * M_PI);

        circlePoint.x = center.x + this->smallRadius * cos(theta) * v1.x
                        + this->smallRadius * sin(theta) * v2.x;
        circlePoint.y = center.y + this->smallRadius * cos(theta) * v1.y
                        + this->smallRadius * sin(theta) * v2.y;
        circlePoint.z = center.z + this->smallRadius * cos(theta) * v1.z
                        + this->smallRadius * sin(theta) * v2.z;

        circle->push_back(circlePoint);
    }

    circle->push_back(*circle->begin());

    if (this->circle == &this->firstCircleStorage)
    {
        this->circle = &this->secondCircleStorage;

        if (!isCircleFirst)
            circlesBinding(&firstCircleStorage, &secondCircleStorage);
    }
    else {
        this->circle = &this->firstCircleStorage;

        if (!isCircleFirst)
            circlesBinding(&secondCircleStorage, &firstCircleStorage);
    }

    if (isCircleFirst)
        isCircleFirst = false;
}

// this method gets two pointers on circles' points containers
// and it binds these two circles by triangles in our soup
void Torus::circlesBinding(const std::vector<Vec> *firstCircle,
                           const std::vector<Vec> *secondCircle)
{
    std::vector<Vec>::const_iterator first, second;
    for (first = firstCircle->cbegin(),
         second = secondCircle->cbegin();
         (first != firstCircle->cend() -1)
         && (second != firstCircle->cend() - 1);
         first++, second++)
    {
        soup.push_back(Vertex{*first, defNormal});
        soup.push_back(Vertex{*(first + 1), defNormal});
        soup.push_back(Vertex{*second, defNormal});

        soup.push_back(Vertex{*(second + 1), defNormal});
        soup.push_back(Vertex{*second, defNormal});
        soup.push_back(Vertex{*(first + 1), defNormal});
    }
}
