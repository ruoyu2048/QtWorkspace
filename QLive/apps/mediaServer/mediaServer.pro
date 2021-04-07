QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = mediaServer
CONFIG(debug, debug|release){
    TARGET = mediaServerd
}
include(../apps.pri)
SOURCES += \
    DynamicRTSPServer.cpp \
    live555MediaServer.cpp

HEADERS += \
    DynamicRTSPServer.hh \
    version.hh


