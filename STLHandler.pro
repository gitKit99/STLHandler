TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Application.cpp \
        Command.cpp \
        Cone.cpp \
        Cube.cpp \
        Cylinder.cpp \
        Parser.cpp \
        STLParser.cpp \
        Sphere.cpp \
        Split.cpp \
        Torus.cpp \
        main.cpp

HEADERS += \
    Application.h \
    Command.h \
    Cone.h \
    Cube.h \
    Cylinder.h \
    Parser.h \
    STLParser.h \
    Sphere.h \
    Split.h \
    Torus.h
