QT += core
QT += gui
QT += testlib

CONFIG += c++11

TARGET = transform
CONFIG += testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    transformskoan.cpp \
    transformations.cpp

HEADERS += \
    transformations.h \
    types.hpp


