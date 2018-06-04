QT += core
QT -= gui

TARGET = neat-env
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    node.cpp \
    genome.cpp \
    gene.cpp \
    src/gene.cpp \
    src/genome.cpp \
    src/main.cpp \
    src/node.cpp

HEADERS += \
    node.h \
    genome.h \
    gene.h \
    include/gene.h \
    include/genome.h \
    include/node.h

QMAKE_CXXFLAGS += -O2 -std=c++14
