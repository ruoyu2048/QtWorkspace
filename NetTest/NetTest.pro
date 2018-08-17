QT += core network sql
QT -= gui

CONFIG += c++11

TARGET = NetTest
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
#    PubFunc.cpp \
#    CTcpClient.cpp \
#    CUdp.cpp \
#    CTcpServer.cpp \
    Libs/CTcpClient/CTcpClient.cpp \
    Libs/CTcpServer/CTcpServer.cpp \
    Libs/CUdp/CUdp.cpp \
    Libs/DB/DB.cpp \
    Libs/Logger/logger.cpp \
    Libs/PubDef/PubFunc.cpp


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
#    DataStruct.h \
#    PubFunc.h \
#    CTcpClient.h \
#    CTcpServer.h \
#    CUdp.h \
#    CTcpServer.h \
    Libs/CTcpClient/CTcpClient.h \
    Libs/CTcpServer/CTcpServer.h \
    Libs/CUdp/CUdp.h \
    Libs/DB/DB.h \
    Libs/Logger/logger.h \
    Libs/PubDef/DataStruct.h \
    Libs/PubDef/PubFunc.h



