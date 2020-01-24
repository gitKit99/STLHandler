#ifndef CYLINDER_H
#define CYLINDER_H

#include "Cone.h"


class Cylinder : public Cone
{
public:
    Cylinder() {};
    const std::string & getName() const override;
protected:
    void createSoup() override;
    Vertex getOnTop(const Vertex&) const;

    const std::string COMMAND = "Cylinder";
};

#endif // CYLINDER_H
