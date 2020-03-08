#-------------------------------------------------
#
# Project created by QtCreator 2020-03-04T11:31:58
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = libHttp
TEMPLATE = lib
CONFIG += staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        libhttp.cpp \
    DeleteRequest.cpp \
    GetRequest.cpp \
    PostJsonRequest.cpp \
    PutJsonRequest.cpp \
    Request.cpp \
    RequestSender.cpp

HEADERS += \
        libhttp.h \
    DeleteRequest.h \
    GetRequest.h \
    PostJsonRequest.h \
    PutJsonRequest.h \
    Request.h \
    RequestSender.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

PROJECT_ROOT = $$OUT_PWD/../../

CONFIG(debug, debug|release) {
        DIR_OUTPUT = $$PROJECT_ROOT/bind
        DEFINES += QHTTP_REQUESTS_DEBUG
}
CONFIG(release, debug|release) {
        DIR_OUTPUT = $$PROJECT_ROOT/bin
}
!contains(BUILD_CONFIG, THIRD_PARTY_BUILD) {
    DESTDIR = $$DIR_OUTPUT
}
