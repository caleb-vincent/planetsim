QT += core gui opengl

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

SOURCES += main.cpp \
    App.cpp

LIBS += -L../FrontEnd -L../BackEnd -lFrontEnd -lBackEnd

INCLUDEPATH += ../BackEnd ../FrontEnd

# Will build the final executable in the main project directory.
TARGET = ../planetsim

HEADERS += \
    App.hpp
