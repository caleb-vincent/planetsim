#-------------------------------------------------
#
# Project created by QtCreator 2013-03-08T22:48:24
#
#-------------------------------------------------
! include( ../common.pri ) {
    error( Could not find the common.pri file! )
}

QT       += opengl gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FrontEnd

SOURCES += MainWindow.cpp \
        GLWidget.cpp \
    DataWidget.cpp \
    AboutDialog.cpp \
    ContextMenu.cpp

HEADERS  += \
            GLWidget.hpp \
    DataWidget.hpp \
    AboutDialog.hpp \
    ContextMenu.hpp \
    MainWindow.hpp

INCLUDEPATH += '../BackEnd'

FORMS += \
    DataWidget.ui \
    MainWindow.ui \
    AboutDialog.ui \
    GLWidget.ui

RESOURCES += \
    FrontEnd.qrc

OTHER_FILES +=
