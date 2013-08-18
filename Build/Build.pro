QT += core gui opengl

TEMPLATE = app

SOURCES += main.cpp 

LIBS += -L../FrontEnd -L../BackEnd -lFrontEnd -lBackEnd

INCLUDEPATH += ../BackEnd ../FrontEnd

# Will build the final executable in the main project directory.
TARGET = ../planetsim
