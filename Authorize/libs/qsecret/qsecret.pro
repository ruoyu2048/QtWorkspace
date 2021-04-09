QT += core
QT -= gui

TARGET = qsecret
CONFIG += c++17
CONFIG += staticlib
CONFIG -= app_bundle

#DEFINES += Qt_SECRET_LIBRARY
#DEFINES += QT_DEPRECATED_WARNINGS
TEMPLATE = lib
#VERSION = 6.1.2

include(../libs.pri)

SOURCES += \
    qaesencryption.cpp \
    qrsaencryption.cpp \
    bigint.cpp \
    mini-gmp.c \
    Codec.cpp

HEADERS += \
    qaesencryption.h \
    qrsaencryption.h \
#    qsecret_global.h \
    bigint.h \
    mini-gmp.h \
    minigmp_global.h \
    Codec.h


