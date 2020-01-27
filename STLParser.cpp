#include "STLParser.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

TriangleSoup STLParser::read(const std::string & filename)
{
    TriangleSoup result;

    std::ifstream file;
    file.open(filename);

    std::string line;
    Vec defNormal{ 0.0, 0.0, 0.0 };
    std::vector<std::string>  words;
    std::getline(file, line);	// ç÷èòóºìî solid model
    line.clear();
    std::getline(file, line); // ç÷èòóºìî facet normal ...
    while (this->split(line)[0] == "facet")
    {
        Vec normal;
        normal.x = atof((this->split(line)[2]).c_str());
        normal.y = atof((this->split(line)[3]).c_str());
        normal.z = atof((this->split(line)[4]).c_str());
        line.clear();
        std::getline(file, line); // ç÷èòóºìî outer loop
        for (int i = 0; i < 3; i++)
        {
            line.clear();
            words.clear();
            std::getline(file, line);
            words = split(line);
            result.push_back(Vertex{ Vec{atof(words[1].c_str()), atof(words[2].c_str()),
                atof(words[3].c_str())}, normal });
        }
        line.clear();

        std::getline(file, line); // ç÷èòóºìî endloop
        line.clear();
        std::getline(file, line);	// ç÷èòóºìî endfacet
        line.clear();

        // ç÷èòóºìî íàñòóïíèé ðÿäîê
        // (íàñòóïíèé facet àáî endsolid)
        std::getline(file, line);
    }
    file.close();
    return result;
}

void STLParser::write(const TriangleSoup & triangleSoup,
    const std::string & filename)
{
    std::ofstream file;
    file.open(filename);
    file << "solid model\n";
    for (it = triangleSoup.cbegin(); it != triangleSoup.cend(); it += 3)
    {
        Vec a{ (it + 1)->pos.x - it->pos.x,
               (it + 1)->pos.y - it->pos.y,
               (it + 1)->pos.z - it->pos.z };

        Vec b{ (it + 2)->pos.x - (it + 1)->pos.x,
               (it + 2)->pos.y - (it + 1)->pos.y,
               (it + 2)->pos.z - (it + 1)->pos.z };
        Vec facetNormal = crossProduct(a, b);

        file << "\tfacet normal ";
        file << facetNormal.x << " " << facetNormal.y << " " <<
            facetNormal.z << std::endl;

        file << "\t\touter loop\n";
        for (int i = 0; i < 3; i++)
        {
            file << "\t\t\tvertex ";
            file << (*(it + i)).pos.x << " " << (*(it + i)).pos.y << " " <<
                (*(it + i)).pos.z << std::endl;
        }

        file << "\t\tendloop\n";
        file << "\tendfacet\n";
    }
    file << "endsolid model\n";
    file.close();
}

std::vector<std::string> STLParser::split(std::string str) {
    std::vector<std::string> result;
    std::string tempStr;
    str.push_back(' ');
    bool isWord = false;
    for (std::string::iterator strIt = str.begin(); strIt != str.end(); strIt++)
    {
        if (*strIt != ' ') {
            tempStr += *strIt;
            if (!isWord)
                isWord = true;
        }
        else if (isWord) {
            result.push_back(tempStr);
            tempStr.clear();
            isWord = false;
        }
    }

    return result;
}

double STLParser::vectorLength(const Vec &v) const
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

void STLParser::vectorNormalize(Vec &v) const
{
    double length;
    length = vectorLength(v);
    double epsi = 0.000000001;
    if (length > epsi)
    {
        v.x /= length;
        v.y /= length;
        v.z /= length;
    }
}

Vec STLParser::crossProduct(Vec a, Vec  b) const
{

    Vec n;
    n.x = a.y * b.z - a.z * b.y;
    n.y = a.z * b.x - a.x * b.z;
    n.z = a.x * b.y - a.y * b.x;

    double epsi = 0.000001;
    vectorNormalize(n);
    if (vectorLength(n) < epsi)
        n = Vec{};

    return n;
}

double STLParser::dotProduct(const Vec &v1, const Vec &v2) const
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// convert value from one interval to another
double STLParser::map(double var, double begin, double end, double targetBegin,
                      double targetEnd) const
{
    double result = targetBegin + ((var - begin) * (targetEnd - targetBegin)) /
        (end - begin);
    return result;
}

Vec STLParser::PointOnPlaneProject(const Vec & point,
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
