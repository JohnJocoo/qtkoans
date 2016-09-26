QT += core
QT -= gui
QT += testlib

CONFIG += c++11
CONFIG += rtti

TARGET = signal_slot
CONFIG += testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += signal_slot_koan.cpp \
    callbacks.cpp \
    message_bus.cpp \
    signal_slot.cpp

HEADERS += \
    callbacks.h \
    message_bus.h \
    signal_slot.h
