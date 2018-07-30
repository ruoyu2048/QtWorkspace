#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T20:19:31
#
#-------------------------------------------------

QT       += core gui sql testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBMaintainTool
TEMPLATE = app


SOURCES += main.cpp\
        DBMaintainToolDlg.cpp \
    BGHandle.cpp \
    DB.cpp

HEADERS  += DBMaintainToolDlg.h \
    DataStruct.h \
    BGHandle.h \
    DB.h \
    GlobalDef.h \
    MacroDef.h

FORMS    += DBMaintainToolDlg.ui
