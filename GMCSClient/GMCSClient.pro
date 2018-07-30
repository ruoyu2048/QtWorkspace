#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T09:50:54
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GMCSClient
TEMPLATE = app


SOURCES += main.cpp\
        GMCSClient.cpp \
    NetworkModule.cpp \
    NetworkCheck.cpp

HEADERS  += GMCSClient.h \
    DataStruct.h \
    GlobalDef.h \
    MacroDef.h \
    NetworkModule.h \
    NetworkCheck.h

FORMS    += GMCSClient.ui

RESOURCES += \
    res.qrc

RC_FILE = GMCSClient.rc
