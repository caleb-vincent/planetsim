#-------------------------------------------------
#
# Project created by QtCreator 2013-03-09T12:29:01
#
#-------------------------------------------------
! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

TARGET = BackEnd

#DEFINES += SIMBACKEND_LIBRARY

SOURCES += \
    Uid.cpp \
    Time.cpp \
    Body.cpp \
    BodyModel.cpp

HEADERS += \
    Vector.hpp \
    Uid.hpp \
    Time.hpp \
    Constants.h \
    Body.hpp \
    BodyModel.hpp
