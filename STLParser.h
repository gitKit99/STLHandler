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

    std::vector<std::string> split(std::string);
    Vec crossProduct(Vec a, Vec b) const;
    double vectorLength(const Vec&) const;
    void vectorNormalize(Vec&) const;
    double dotProduct(const Vec&, const Vec&) const;
    double map(double var, double begin, double end, double targetBegin,
               double targetEnd) const;
    Vec PointOnPlaneProject(const Vec & point, const Vec & pointOnPlane,
                                  const Vec & planeNormal) const;
private:
    TriangleSoup::const_iterator it;
};

#endif // STLPARSER_H
