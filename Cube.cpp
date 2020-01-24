#include "Cube.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <fstream>

const std::string& Cube::getName() const {
    return this->COMMAND_NAME;
}

int Cube::execute(const std::map<std::string, std::string> &args) {
    std::cout << "Creation cube" << std::endl;

    int argCheck = checkInput(args);

    if (argCheck != 0)
        return argCheck;

    this->init(atof((args.at("L")).c_str()), parseCoord(args.at("origin")),
        args.at("filepath"));
    this->calcVertices();
    this->createSoup();
    std::cout << "Count of triangles " << this->soup.size() / 3 << std::endl;

    parser.write(this->soup, this->outFile);

    return 0;
}

void Cube::init(double L, Vec  origin, std::string  filename) {
    this->L = L;

    this->origin.x = origin.x;
    this->origin.y = origin.y;
    this->origin.z = origin.z;

    this->outFile = filename;


    std::cout << this->L << std::endl << this->origin.x << " " <<
        this->origin.y << " " << this->origin.z << std::endl <<
        this->outFile << std::endl;
}

Vec Cube::parseCoord(std::string  coords)
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

void Cube::calcVertices()
{

    this->mesh.push_back(Vertex{ this->origin, defNormal });
    this->mesh.push_back(Vertex{
        Vec{origin.x + L, origin.y, origin.z},
        defNormal });
    this->mesh.push_back(Vertex{
        Vec{origin.x + L, origin.y, origin.z - L},
        defNormal });
    this->mesh.push_back(Vertex{
        Vec{origin.x, origin.y, origin.z - L},
        defNormal });

    this->mesh.push_back(Vertex{
        Vec{origin.x, origin.y + L, origin.z},
        defNormal });
    this->mesh.push_back(Vertex{
        Vec{origin.x + L, origin.y + L, origin.z},
        defNormal });
    this->mesh.push_back(Vertex{
        Vec{origin.x + L, origin.y + L, origin.z - L},
        defNormal });
    this->mesh.push_back(Vertex{
        Vec{origin.x, origin.y + L, origin.z - L},
        defNormal });
}

void Cube::createSoup() {
    soup.push_back(mesh[0]);
    soup.push_back(mesh[5]);
    soup.push_back(mesh[4]);

    soup.push_back(mesh[0]);
    soup.push_back(mesh[1]);
    soup.push_back(mesh[5]);

    soup.push_back(mesh[1]);
    soup.push_back(mesh[6]);
    soup.push_back(mesh[5]);

    soup.push_back(mesh[1]);
    soup.push_back(mesh[2]);
    soup.push_back(mesh[6]);

    soup.push_back(mesh[2]);
    soup.push_back(mesh[7]);
    soup.push_back(mesh[6]);

    soup.push_back(mesh[2]);
    soup.push_back(mesh[3]);
    soup.push_back(mesh[7]);

    soup.push_back(mesh[3]);
    soup.push_back(mesh[4]);
    soup.push_back(mesh[7]);

    soup.push_back(mesh[3]);
    soup.push_back(mesh[0]);
    soup.push_back(mesh[4]);

    soup.push_back(mesh[4]);
    soup.push_back(mesh[6]);
    soup.push_back(mesh[7]);

    soup.push_back(mesh[4]);
    soup.push_back(mesh[5]);
    soup.push_back(mesh[6]);

    soup.push_back(mesh[2]);
    soup.push_back(mesh[1]);
    soup.push_back(mesh[0]);

    soup.push_back(mesh[2]);
    soup.push_back(mesh[0]);
    soup.push_back(mesh[3]);
}

int Cube::checkInput(const std::map<std::string, std::string>& input)
{
    if (input.find("L") == input.end() || input.find("origin") == input.end() ||
        input.find("filepath") == input.end())
        return 3;

    if (atof(input.at("L").c_str()) <= 0)
        return 1;

    return 0;
}


