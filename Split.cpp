#include "Split.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cmath>

struct vertPos {
    Vertex vert;
    double angle;
};

const std::string & Split::getName() const
{
    return this->COMMAND_NAME;
}

int Split::execute(const std::map<std::string, std::string>& args)
{
    int inputCheck = checkInput(args);
    if (inputCheck != 0)
        return inputCheck;

    this->init(args.at("input"), parseCoord(args.at("origin")),
        parseCoord(args.at("direction")), args.at("output1"),
        args.at("output2"));

    this->sourceSoup = parser.read(this->input); // ç÷èòóºìî âõ³äíèé ôàéë
    int result = splitSoup();
    if (result != 0)
    {
        parser.write(sourceSoup, output1);
        parser.write(sourceSoup, output2);
    }
    else {
        parser.write(firstSoup, output1);
        parser.write(secondSoup, output2);
    }


    std::cout << "Count of triangles " << sourceSoup.size() / 3 << std::endl;

    return result;
}

int Split::checkInput(const std::map<std::string, std::string>& input)
{
    // ïåðåâ³ðêà íà ³ñíóâàííÿ óñ³õ àðãóìåíò³â
    if (input.find("input") == input.end() || input.find("origin") == input.end() ||
        input.find("direction") == input.end() ||
        input.find("output1") == input.end() || input.find("output2") == input.end())
        return 3;

    std::ifstream testInput(input.at("input")), testOutput1(input.at("output1")),
        testOutput2(input.at("output2"));

    return 0;
}

void Split::init(std::string input, Vec origin, Vec direction, std::string output1, std::string output2)
{
    this->input = input;
    this->origin = Vec{ origin.x, origin.y, origin.z };
    this->direction = Vec{ direction.x, direction.y, direction.z };
    this->output1 = output1;
    this->output2 = output2;
}

Vec Split::parseCoord(std::string  coords)
{
    coords.erase(std::remove(coords.begin(), coords.end(), '('),
        coords.end());
    coords.erase(std::remove(coords.begin(), coords.end(), ')'),
        coords.end());
    std::replace(coords.begin(), coords.end(), ',', ' ');

    std::vector<std::string> coordVec = split(coords);

    return Vec{ atof(coordVec[0].c_str()), atof(coordVec[1].c_str()), atof(coordVec[2].c_str()) };
}

// ä³ëåííÿ ô³ãóðè íà äâ³ ÷àñòèíè, òà ñòâîðåííÿ ë³í³¿ ðîçìåæóâàííÿ
int Split::splitSoup()
{
    std::vector<Vertex>::const_iterator it;
    double res;
    // ïðîõîäèìñÿ ïî êîæíîìó òðèêóòíèêó
    bool wasSwapping = false;
    bool wasIntersected = false;
    int boundPtsCount = 0;
    for (it = sourceSoup.cbegin(); it != sourceSoup.cend(); it += 3)
    {
        double planeSide[3];
        // ïîçèö³ÿ êîæíî¿ òî÷êè òðèêóòíèêà â³äíîñíî ïëîùèíè
        // çë³âà, ñïðàâà, íà  ïëîùèí³
        planeSide[0] = planeEquation(it->pos);
        planeSide[1] = planeEquation((it + 1)->pos);
        planeSide[2] = planeEquation((it + 2)->pos);

        // òóò çä³éñíþºòüñÿ ïåðåâ³ðêà ÷è ³ñíóþ÷à òî÷êà òðèêóòíèêà
        //ëåæèòü íà ïëîùèí³
        for (int i = 0; i < 3; i++)
        {
            // ÿêùî òî÷êà ëåæèòü íà ïëîùèí³
            if (planeSide[i] == 0)
            {
                // ÿêùî âîíà çóñòð³ëàñÿ âïåðøå, äîäàºìî ¿¿ äî òî÷îê ïîâåðõí³
                if (!isContain(boundPoints, (it + i)->pos))
                {
                    boundPoints.push_back(*(it + i));
                    boundPtsCount++;
                }
            }
            else res = planeSide[i];
        }
        // ÿêùî ïîòî÷íèé òðèêóòíèê ïåðåòèíàºòüñÿ ïëîùèíîþ
        if ((planeSide[0] * planeSide[1] < 0)
            || (planeSide[0] * planeSide[2] < 0)
            || (planeSide[1] * planeSide[2] < 0))
        {
            res = 0;
            wasIntersected = true;
            // ââîäèìî äîäàòêîâ³ çì³íí³
            Vec v0 = it->pos;
            Vec v1 = (it + 1)->pos;
            Vec v2 = (it + 2)->pos;
            // vMaj - âåðøèíè, ùî ïî îäíó ñòîðîíó, vMin - îäíà âåðøèíà ùî íà
            // ³íø³é ñòîðîí³
            Vec vMaj1{ 0, 0, 0 }, vMaj2{ 0, 0, 0 }, vMin{ 0, 0, 0 };

            // ÿêùî v0 ³ v1 ïî ð³çíó ñòîðîíó
            if (planeSide[0] * planeSide[1] < 0)
            {
                if (planeSide[1] * planeSide[2] < 0) {
                    vMaj1 = v2;
                    vMaj2 = v0;
                    vMin = v1;
                }
                else
                {
                    vMaj1 = v1;
                    vMaj2 = v2;
                    vMin = v0;
                }
            }
            else
            {
                vMaj1 = v0;
                vMaj2 = v1;

                vMin = v2;
            }
            Vec vOnPl1 = linePlaneIntersect(vMaj2, vMin);
            TriangleSoup* soupPtr;
            double side;
            if (planeEquation(vMaj1) != 0)
                side = planeEquation(vMaj1);
            else side = planeEquation(vMaj2);
            if (side < 0)
                soupPtr = &firstSoup;
            else soupPtr = &secondSoup;
            soupPtr->push_back(Vertex{ vMaj1, defNormal });
            soupPtr->push_back(Vertex{ vMaj2, defNormal });
            soupPtr->push_back(Vertex{ vOnPl1, defNormal });

            Vec vOnPl2 = linePlaneIntersect(vMin, vMaj1);
            soupPtr->push_back(Vertex{ vMaj1, defNormal });
            soupPtr->push_back(Vertex{ vOnPl1, defNormal });
            soupPtr->push_back(Vertex{ vOnPl2, defNormal });

            if (soupPtr == &firstSoup)
                soupPtr = &secondSoup;
            else soupPtr = &firstSoup;

            soupPtr->push_back(Vertex{ vOnPl2, defNormal });
            soupPtr->push_back(Vertex{ vOnPl1, defNormal });
            soupPtr->push_back(Vertex{ vMin, defNormal });

            bool isEdge = true;
            if (!isContain(boundPoints, vOnPl1))
            {
                boundPoints.push_back(Vertex{ vOnPl1, defNormal });
                boundPtsCount++;
            }
            else isEdge = false;
            if (!isContain(boundPoints, vOnPl2))
            {
                boundPoints.push_back(Vertex{ vOnPl2, defNormal });
                boundPtsCount++;
            }
            else if (isEdge)
                isEdge = false;
            if (isEdge && !wasSwapping)
            {
                if (side < 0)
                    isDirectByPass = false;
                else isDirectByPass = true;

                std::swap(boundPoints[0], boundPoints[boundPoints.size() - 2]);
                std::swap(boundPoints[1], boundPoints[boundPoints.size() - 1]);
                wasSwapping = true;
            }
        }
        else
        {
            TriangleSoup* soupPtr;
            if (res < 0)
                soupPtr = &firstSoup;
            else soupPtr = &secondSoup;

            soupPtr->push_back(*it);
            soupPtr->push_back(*(it + 1));
            soupPtr->push_back(*(it + 2));
        }
    }

    if (!wasIntersected)
        return 4;
    createSurfaceSoup();

    if (isDirectByPass)
    {
        pushBackVector(firstSoup, surfaceSoup);
        std::reverse(surfaceSoup.begin(), surfaceSoup.end());
        pushBackVector(secondSoup, surfaceSoup);
    }

    else {
        pushBackVector(secondSoup, surfaceSoup);
        std::reverse(surfaceSoup.begin(), surfaceSoup.end());
        pushBackVector(firstSoup, surfaceSoup);
    }

    return 0;
}

double Split::planeEquation(const Vec &p)
{
    return direction.x * (p.x - origin.x) + direction.y * (p.y - origin.y) +
        direction.z * (p.z - origin.z);
}

Vec Split::linePlaneIntersect(const Vec & p0, const Vec & p1)
{
    Vec v{p1.x - p0.x, p1.y - p0.y, p1.z - p0.z}; // íàïðÿìíèé âåêòîð ïðÿìî¿
    Vec w{origin.x - p0.x, origin.y - p0.y, origin.z - p0.z};
    Vec intersectPoint{};
    double denom = dotProduct(this->direction, v);
    if (denom != 0)
    {
        double t = dotProduct(this->direction, w) / denom;
        intersectPoint.x = p0.x + v.x * t;
        intersectPoint.y = p0.y + v.y * t;
        intersectPoint.z = p0.z + v.z * t;
    }
    else std::cout << "Point of intersection has not found" << std::endl;
    return intersectPoint;
}

double Split::dotProduct(const Vec & v1, const Vec & v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// ñòâîðåííÿ òð³àíãóëüîâàíî¿ ïîâåðõí³, ùî çàêðèâàº ä³ðêè â îá'ºêò³, óòâîðåí³
// ïðè ðîçð³ç³
void Split::createSurfaceSoup()
{
    sortBoundVertices();
    triangSurface();
}

void Split::pushBackVector(TriangleSoup &dest, const TriangleSoup &vector)
{
    TriangleSoup::const_iterator it;
    for (it = vector.cbegin(); it != vector.cend(); it++)
        dest.push_back(*it);
}

// äîâæèíà âåêòîðà
double Split::vectorLength(const Vec &vec)
{
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

double Split::vectorsAngle(const Vec &a, const Vec &b)
{
    double aLength = vectorLength(a);
    double bLength = vectorLength(b);

    if (aLength * bLength != 0)
        return dotProduct(a, b) / (aLength * bLength);
    else
    {
        return -1;
        std::cout << "Length of vector was 0/n";
    }
}

double Split::distBtwnPoints(const Vec &p1, const Vec &p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
}

bool Split::isContain(const TriangleSoup &soup, const Vec &vec)
{
    double epsi = 0.000001;
    for (const Vertex& vert : soup) {
        if ((fabs(vert.pos.x - vec.x) < epsi) && (fabs(vert.pos.y - vec.y) < epsi) &&
            (fabs(vert.pos.z - vec.z) < epsi))
            return true;
    }
    return false;
}

void Split::sortBoundVertices()
{
    std::vector<vertPos> vertPostns;

    TriangleSoup::iterator start, last;
    start = boundPoints.begin();
    last = boundPoints.begin() + 1;

    Vec v{ start->pos.x - last->pos.x,
          start->pos.y - last->pos.y,
          start->pos.z - last->pos.z };

    //std::cout << "Vector v (" << v.x << ", " << v.y << ", " << v.z << ")\n";

    Vertex lastVert = Vertex{ Vec{last->pos.x, last->pos.y, last->pos.z},
        defNormal };
    boundPoints.erase(last);

    for (Vertex vertex : boundPoints)
    {
        // çàïèñóºìî ïàðè âåðøèíà-êóò
        Vec w{ vertex.pos.x - lastVert.pos.x,
              vertex.pos.y - lastVert.pos.y,
              vertex.pos.z - lastVert.pos.z };
        double angle = vectorsAngle(v, w);
        //std::cout << angle << std::endl;
        vertPostns.push_back(vertPos{ vertex, angle });
    }

    // ñîðòóºìî âåðøèíè çà êóòîì
    for (std::vector<vertPos>::iterator i = vertPostns.begin() + 1;
        i != vertPostns.end() - 1; i++)
    {
        for (std::vector<vertPos>::iterator j = i + 1;
            j != vertPostns.end(); j++)
        {
            // ñîðòóºìî çà ñïàäàííÿì êóòà
            if (i->angle < j->angle)
                std::swap(*i, *j);
            else if (fabs(i->angle - j->angle) < 0.00001)
            {
                // ÿêùî êóòè ì³æ âåêòîðàìè îäíàêîâ³
                double iDist = distBtwnPoints(i->vert.pos,
                    lastVert.pos);
                double jDist = distBtwnPoints(j->vert.pos,
                    lastVert.pos);
                // ÿêùî êóò ãîñòðèé, ïåðøó äîäàºìî áëèæíþ òî÷êó,
                // â ³íøîìó âèïàäêó (òóïèé ÷è ïðÿìîêóòíèé) - äàëüíþ
                if (((i->angle > 0) && (iDist > jDist)) || ((i->angle <= 0) &&
                    (iDist < jDist)))
                    std::swap(*i, *j);
            }
        }
    }

    vertPostns.push_back(vertPos{ lastVert, -1 });
    boundPoints.clear();

    for (std::vector<vertPos>::iterator it = vertPostns.begin(); it !=
        vertPostns.end(); it++)
    {
        boundPoints.push_back(it->vert);
    }

    /*std::cout << "Angles\n";
    for (std::vector<vertPos>::iterator it = vertPostns.end() - 5; it !=
        vertPostns.end(); it++)
    {
        Vec new_w{ it->vert.pos.x - lastVert.pos.x,
                  it->vert.pos.y - lastVert.pos.y,
                  it->vert.pos.z - lastVert.pos.z };
        std::cout << "new_w length = " << vectorLength(new_w) << std::endl;
        std::cout << it->angle << std::endl;
    }*/
}


void Split::triangSurface()
{
    Vertex center{};
    for (Vertex v : boundPoints)
    {
        center.pos.x += v.pos.x;
        center.pos.y += v.pos.y;
        center.pos.z += v.pos.z;
    }

    center.pos.x /= boundPoints.size();
    center.pos.y /= boundPoints.size();
    center.pos.z /= boundPoints.size();

    //std::cout << "Triangles:\n";
    for (TriangleSoup::iterator it = boundPoints.begin() + 1;
        it != boundPoints.end(); it++)
    {
        surfaceSoup.push_back(center);
        //std::cout << "center\n";
        surfaceSoup.push_back(*it);

        //std::cout << it->pos.x << ", " << it->pos.y << ", " << it->pos.z << std::endl;
        surfaceSoup.push_back(*(it - 1));
        //std::cout << (it - 1)->pos.x << ", " << (it - 1)->pos.y << ", " <<
            //(it - 1)->pos.z << std::endl << std::endl;

    }

    surfaceSoup.push_back(center);
    surfaceSoup.push_back(boundPoints[0]);
    surfaceSoup.push_back(boundPoints.back());
}

void Split::outputSurfPoints()
{
    for (Vertex v : boundPoints)
    {
        std::cout << "(" << v.pos.x << ", " << v.pos.y << ", " << v.pos.z <<
            ")\n";
    }
}

/*double Split::detMatr3n(double a[3][3]) {
    double firstStar = ((a[0][0] * a[1][1] * a[2][2]) + (a[0][1] * a[1][2] * a[2][0]) + (a[0][2] * a[1][0] * a[2][1]));
    double secStar = ((a[0][2] * a[1][1] * a[2][0]) + (a[0][1] * a[1][0] * a[2][2]) + (a[0][0] * a[1][2] * a[2][1]));
    return firstStar - secStar;
}

bool Split::isCounterClockwise(const Vec & v0, const Vec & v1, const Vec & v2)
{
    double matrix[3][3]{ {v0.x, v0.y, v0.z},
                        {v1.x, v1.y, v1.z},
                        {v2.x, v2.y, v2.z} };
    double det = detMatr3n(matrix);
    std::cout << "det = " << det << std::endl;
    if (det > 0)
        return true;
    return false;
}
*/
