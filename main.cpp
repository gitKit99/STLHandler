#include <iostream>

#include "Application.h" // підключення файлу програми

#include "Cube.h" // підключення команд
#include "Sphere.h"
#include "Split.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Torus.h"

int main(int argc, char *argv[])
{
    Application app;

    app.registerCommand(std::unique_ptr<Cube>(new Cube()));
    app.registerCommand(std::unique_ptr<Sphere>(new Sphere()));
    app.registerCommand(std::unique_ptr<Split>(new Split()));

    app.registerCommand(std::unique_ptr<Cone>(new Cone()));
    app.registerCommand(std::unique_ptr<Cone>(new Cylinder()));
    app.registerCommand(std::unique_ptr<Torus>(new Torus()));

    return app.execute(argc, argv);
}
