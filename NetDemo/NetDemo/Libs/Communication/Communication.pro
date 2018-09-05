#-------------------------------------------------
#
# Project created by QtCreator 2018-08-23T08:49:45
#
#-------------------------------------------------

QT       += network serialport

QT       -= gui

TARGET = Communication
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CTcpClient.cpp \
    CTcpServer.cpp \
    CUdp.cpp \
    Communication.cpp \
    CTcpThread.cpp \
    CSerialPort.cpp \
    CDataPacket*.cpp \
    CommunicationCfg.cpp

HEADERS += \
    CTcpClient.h \
    CTcpServer.h \
    DataStruct.h \
    CUdp.h \
    Communication.h \
    CTcpThread.h \
    CSerialPort.h \
    CDataPacket*.h \
    CommunicationCfg.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

include (../Libs.pri)
INCLUDEPATH  += ./
