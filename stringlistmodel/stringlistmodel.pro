QT += core
QT -= gui
QT += testlib

CONFIG += c++11

TARGET = stringlistmodel
CONFIG += testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += listmodelfromscratch.cpp \
    stringlistmodel.cpp

HEADERS += \
    stringlistmodel.h
