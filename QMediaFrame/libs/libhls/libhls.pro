TARGET = libhls
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt
include(../libs.pri)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

INCLUDEPATH += $$PWD/../libmov/include
DEPENDPATH += $$PWD/../libmov/include

INCLUDEPATH += $$PWD/../libmpeg/include
DEPENDPATH += $$PWD/../libmpeg/include

HEADERS += \
    include/hls-fmp4.h \
    include/hls-m3u8.h \
    include/hls-media.h \
    include/hls-param.h \
    source/hls-h264.h \
    source/list.h

SOURCES += \
    source/hls-fmp4.c \
    source/hls-m3u8.c \
    source/hls-media.c



