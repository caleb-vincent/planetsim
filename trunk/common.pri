INCLUDEPATH += . ..
WARNINGS += -Wall

TEMPLATE = lib

CONFIG += staticlib debug

linux-g++ {
    system( echo |gcc -x c++ - -E -std=c++11 2>&1 | grep 'c++11'  ) {
            system(  echo |gcc -x c++ - -E -std=c++0x 2>&1 | grep 'c++0x'  ){
                error( g++ does not support C++11 or C++0x )
            }
            else{
                message( g++ supports C++0x)
                QMAKE_CXXFLAGS += -std=c++0x
            }
    }
    else{
        message( g++ supports C++11)
        QMAKE_CXXFLAGS += -std=c++11
    }
}
else{
    message(TODO: Sort out non-Linux support )
}



