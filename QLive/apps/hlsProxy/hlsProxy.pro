QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = hlsProxy
CONFIG(debug, debug|release){
    TARGET = hlsProxyd
}
include(../apps.pri)
SOURCES += \
    live555HLSProxy.cpp
