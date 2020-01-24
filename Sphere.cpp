#include "Sphere.h"

#include <iostream>
#include <algorithm>
#include <math.h>
#include <fstream>


const std::string & Sphere::getName() const
{
    return this->COMMAND_NAME;
}

int Sphere::execute(const std::map<std::string, std::string>& args)
{
    std::cout << "Creation sphere" << std::endl;

    int checkRes = checkInput(args);

    if (checkRes != 0)
        return checkRes;

    this->init(atof((args.at("R")).c_str()), parseCoord(args.at("origin")),
        args.at("filepath"));
    this->calcVertices();
    this->createSoup();
    std::cout << "Count of triangles " << this->soup.size() / 3 << std::endl;

    parser.write(this->soup, this->outFile);

    return 0;
}

void Sphere::init(double R, Vec origin, std::string filename)
{
    this->R = R;

    this->origin.x = origin.x;
    this->origin.y = origin.y;
    this->origin.z = origin.z;

    this->outFile = filename;


    std::cout << this->R << std::endl << this->origin.x << " " <<
        this->origin.y << " " << this->origin.z << std::endl <<
        this->outFile << std::endl;
}

Vec Sphere::parseCoord(std::string coords)
{
    coords.erase(std::remove(coords.begin(), coords.end(), '('),
        coords.end());
    coords.erase(std::remove(coords.begin(), coords.end(), ')'),
        coords.end());
    std::replace(coords.begin(), coords.end(), ',', ' ');

    std::vector<std::string> coordVec = split(coords);

    return Vec{ atof(coordVec[0].c_str()), atof(coordVec[1].c_str()),
        atof(coordVec[2].c_str()) };
}

void Sphere::calcVertices()
{
    pointsCount = 3 * tot * tot;
    // create matrix for
    mesh = new Vertex*[tot];
    for (int i = 0; i < tot; i++)
        mesh[i] = new Vertex[tot];

    //  çàïîâíþºìî ñòâîðåíó ñ³òêó âåðøèí ¿õí³ìè êîîðäèíàòàìè
    for (int i = 0; i < tot; i++) // ðóõ â³ä "ïîëþñà" äî "ïîëþñà"
    {
        // ïåðåòâîðåííÿ íîìåðó ñåêòîðà ó â³äïîâ³äíèé êóò
        double lat = map(i, 0, tot - 1, 0, M_PI);
        for (int j = 0; j < tot; j++) // ðóõ âçäîâæ îäí³º¿ ïàðàëåë³
        {
            double lon = map(j, 0, tot, 0, 2 * M_PI);
            double x = origin.x + R * sin(lat) * cos(lon);
            double y = origin.y + R  * sin(lat) * sin(lon);
            double z = origin.z + R * cos(lat);

            mesh[i][j] = Vertex{ x, y, z };
        }
    }

}

void Sphere::createSoup()
{
    int next_i, next_j;
    for (int i = 0; i < tot - 1; i++)
    {
        for (int j = 0 ; j < tot; j++)
        {
            if (i == tot - 1 && j == tot - 1)
                break;
            next_i = i + 1;
            if (j == tot - 1) // äëÿ çàìêíåííÿ ñôåðè
                next_j = 0;
            else next_j = j + 1;

            // âíåñåííÿ êîæíîãî ñåêòîðó â ñóï òðèêóòíèê³â
            soup.push_back(mesh[i][j]); // âåðõí³é òðèêóòíèê
            soup.push_back(mesh[next_i][next_j]);
            soup.push_back(mesh[i][next_j]);

            soup.push_back(mesh[i][j]); // íèæí³é òðèêóòíèê
            soup.push_back(mesh[next_i][j]);
            soup.push_back(mesh[next_i][next_j]);
        }
    }
}

int Sphere::checkInput(const std::map<std::string, std::string>& input)
{
    // ïåðåâ³ðêà íà ³ñíóâàííÿ óñ³õ àðãóìåíò³â
    if (input.find("R") == input.end() || input.find("origin") == input.end() ||
        input.find("filepath") == input.end())
        return 3;

    // ïåðåâ³ðêà ÷è êîðåêòíî çàäàíè1 ðàä³óñ ñôåðè
    if (atof(input.at("R").c_str()) <= 0)
        return 1;

    std::ifstream ftest;
    ftest.open(input.at("filepath"));

   /* if (!ftest.good())
        return 2;*/

    return 0;
}

// ïåðåòâîðåííÿ ÷èñëà ç îäíîãî ³íòåðâàëó ó â³äïîâ³äíå ç ³íøîãî ³íòåðâàëó
double Sphere::map(const double & var, const double & begin, const double & end,
    const double & targetBegin, const double & targetEnd)
{
    double result = targetBegin + ((var - begin) * (targetEnd - targetBegin)) /
        (end - begin);
    return result;
}

