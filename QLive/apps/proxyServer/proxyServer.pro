QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

TARGET = proxyServer
CONFIG(debug, debug|release){
    TARGET = proxyServerd
}
include(../apps.pri)
SOURCES += \
    live555ProxyServer.cpp
