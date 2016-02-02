QT += core
QT -= gui
QT += xml
QT += testlib

CONFIG += c++11

TARGET = xmltreemodel
CONFIG += testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += xmltreemodel.cpp \
    treemodelfromscratch.cpp

HEADERS += \
    xmltreemodel.h

RESOURCES += \
    resource.qrc
