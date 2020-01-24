#ifndef STLPARSER_H
#define STLPARSER_H

#include <vector>
#include <string>

struct Vec { double x, y, z; }; // ñòðóêòóðà 3D âåêòîðà

struct Vertex {		// ñòðóêòóðà âåðøèíè, ùî ì³ñòèòü ¿¿ ïîëîæåííÿ òà íîðìàëü
    Vec pos;
    Vec normal;
};

using TriangleSoup = std::vector<Vertex>;

class STLParser
{
public:
    TriangleSoup read(const std::string& filename);
    void write(const TriangleSoup& triangleSoup, const std::string& filename);
private:
    TriangleSoup::const_iterator it;
    std::vector<std::string> split(std::string);
    Vec crossProduct(Vec a, Vec b);
    double vectorLength(const Vec&);
    void vectorNormalize(Vec&);
};

#endif // STLPARSER_H
