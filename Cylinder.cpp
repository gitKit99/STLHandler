#include "Cylinder.h"


const std::string &Cylinder::getName() const
{
    return this->COMMAND;
}

void Cylinder::createSoup()
{
    Vertex baseCenter{this->base.center, defNormal};

    this->mesh.push_back(*(this->mesh.cbegin() + 2));

    // pass by ring vertices and bind them with top and circle center
    for (TriangleSoup::const_iterator it = this->mesh.cbegin() + 3;
         it != this->mesh.cend();
         it++) {
        // bottom circle
        this->soup.push_back(*it);
        this->soup.push_back(*(it - 1));
        this->soup.push_back(baseCenter);

        // side
        this->soup.push_back(this->getOnTop(*it));
        this->soup.push_back(this->getOnTop(*(it - 1)));
        this->soup.push_back(*it);

        this->soup.push_back(*it);
        this->soup.push_back(*(it - 1));
        this->soup.push_back(this->getOnTop(*(it - 1)));

        // top circle
        this->soup.push_back(this->getOnTop(*it));
        this->soup.push_back(this->getOnTop(*(it - 1)));
        this->soup.push_back(this->getOnTop(baseCenter));
    }
}

Vertex Cylinder::getOnTop(const Vertex &vertex) const
{
    static Vec heightShift{this->origin.x - this->base.center.x,
                           this->origin.y - this->base.center.y,
                           this->origin.z - this->base.center.z};

    return Vertex{Vec{vertex.pos.x + heightShift.x,
                      vertex.pos.y + heightShift.y,
                      vertex.pos.z + heightShift.z}, vertex.normal};
}
