TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

include(../apps.pri)

SOURCES += \
        main.cpp \
    base64.cpp \
    Crypto.cpp

HEADERS += \
    base64.h \
    Crypto.h
