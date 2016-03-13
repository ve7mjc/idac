QT += core network
QT -= gui

DEFINES += QMQTT_LIBRARY
include(../lib/qmqtt/qmqtt.pri)

LIBS += -L/usr/lib/x86_64-linux-gnu/qca-qt5 -lqca-qt5
INCLUDEPATH += /usr/include/qt5/Qca-qt5/QtCrypto

CONFIG += c++11

TARGET = oacim
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    partition.cpp \
    mqttlogger.cpp \
    config.cpp \
    user.cpp \
    token.cpp \
    controller.cpp \
    lock.cpp \
    state.cpp

HEADERS += \
    partition.h \
    mqttlogger.h \
    config.h \
    user.h \
    token.h \
    controller.h \
    lock.h \
    state.h
