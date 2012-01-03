#-------------------------------------------------
#
# Project created by QtCreator 2011-03-27T06:07:36
#
#-------------------------------------------------

TARGET = battlelogs
TEMPLATE = lib
DESTDIR = ../../bin/serverplugins
DEFINES += BATTLELOGS_LIBRARY
QT += gui core

SOURCES += battlelogs.cpp \
    battleserverlog.cpp

HEADERS += battlelogs.h\
        BattleLogs_global.h \
    ../Server/plugininterface.h \
    ../Server/battleinterface.h \
    battleserverlog.h \
    ../Shared/battlecommands.h
LIBS += -L../../bin \
    -lpokemonlib \
    -lutilities \
    -lbattlelib
QMAKE_CXXFLAGS += "-std=c++0x -U__STRICT_ANSI__"

macx {
   INCLUDEPATH += /usr/local/gcc-4.6.2/include
   LIBS += -L/usr/local/gcc-4.6.2/lib
}

