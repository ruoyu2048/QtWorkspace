TARGET = libflv
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib

#CONFIG -= qt

include(../libs.pri)

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

HEADERS += \
    include/amf0.h \
    include/amf3.h \
    include/flv-demuxer.h \
    include/flv-muxer.h \
    include/flv-parser.h \
    include/flv-proto.h \
    include/flv-reader.h \
    include/flv-writer.h \
    include/mp3-header.h \
    include/mpeg4-aac.h \
    include/mpeg4-avc.h \
    include/mpeg4-bits.h \
    include/mpeg4-hevc.h

SOURCES += \
    source/amf0.c \
    source/amf3.c \
    source/flv-demuxer.c \
    source/flv-demuxer-script.c \
    source/flv-muxer.c \
    source/flv-parser.c \
    source/flv-reader.c \
    source/flv-writer.c \
    source/hevc-annexbtomp4.c \
    source/hevc-mp4toannexb.c \
    source/mp3-header.c \
    source/mpeg4-aac.c \
    source/mpeg4-aac-asc.c \
    source/mpeg4-annexbtomp4.c \
    source/mpeg4-avc.c \
    source/mpeg4-hevc.c \
    source/mpeg4-mp4toannexb.c


