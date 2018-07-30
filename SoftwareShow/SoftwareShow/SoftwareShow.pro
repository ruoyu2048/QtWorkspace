#-------------------------------------------------
#
# Project created by QtCreator 2016-07-07T14:10:51
#
#-------------------------------------------------

QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoftwareShow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filtermodel.cpp \
    filterwidget.cpp \
    DB.cpp \
    nodestatetree.cpp \
    nodefiltermodel.cpp

HEADERS  += mainwindow.h \
    filtermodel.h \
    prodef.h \
    filterwidget.h \
    DB.h \
    nodestatetree.h \
    nodefiltermodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    softwareshow.qrc
