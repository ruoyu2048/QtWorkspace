#-------------------------------------------------
#
# Project created by QtCreator 2017-06-01T15:14:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RabbitClient
CONFIG(debug, debug|release){
    TARGET = RabbitClient
}
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        QRabbitMQ.cpp

HEADERS  += mainwindow.h \
        QRabbitMQ.h

FORMS    += mainwindow.ui

include(../apps.pri)
