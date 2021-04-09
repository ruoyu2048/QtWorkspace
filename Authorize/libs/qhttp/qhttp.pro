QT       += core network
QT       -= gui

TARGET    = qhttp
TEMPLATE  = lib
CONFIG += staticlib

#include(../commondir.pri)

include(../libs.pri)

DEFINES       *= QHTTP_MEMORY_LOG=0
win32:DEFINES *= QHTTP_EXPORT

DEFINES += QHTTP_HAS_CLIENT

INCLUDEPATH += $$PWD/3rdparty

# Joyent http_parser
SOURCES  += $$PWD/3rdparty/http-parser/http_parser.c
HEADERS  += $$PWD/3rdparty/http-parser/http_parser.h

SOURCES  += \
    qhttpabstracts.cpp \
    qhttpserverconnection.cpp \
    qhttpserverrequest.cpp \
    qhttpserverresponse.cpp \
    qhttpserver.cpp

HEADERS  += \
    qhttpfwd.hpp \
    qhttpabstracts.hpp \
    qhttpserverconnection.hpp \
    qhttpserverrequest.hpp \
    qhttpserverresponse.hpp \
    qhttpserver.hpp

contains(DEFINES, QHTTP_HAS_CLIENT) {
    SOURCES += \
        qhttpclientrequest.cpp \
        qhttpclientresponse.cpp \
        qhttpclient.cpp

    HEADERS += \
        qhttpclient.hpp \
        qhttpclientresponse.hpp \
        qhttpclientrequest.hpp
}
