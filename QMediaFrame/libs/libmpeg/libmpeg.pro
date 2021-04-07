TARGET = libmpeg
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt
include(../libs.pri)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

HEADERS += \
    include/mpeg-element-descriptor.h \
    include/mpeg-pes-proto.h \
    include/mpeg-ps.h \
    include/mpeg-ps-proto.h \
    include/mpeg-ts.h \
    include/mpeg-ts-proto.h \
    include/mpeg-types.h \
    source/mpeg-util.h

SOURCES += \
    source/mpeg-crc32.c \
    source/mpeg-element-descriptor.c \
    source/mpeg-packet.c \
    source/mpeg-pack-header.c \
    source/mpeg-pat.c \
    source/mpeg-pes.c \
    source/mpeg-pmt.c \
    source/mpeg-psd.c \
    source/mpeg-ps-dec.c \
    source/mpeg-ps-enc.c \
    source/mpeg-psm.c \
    source/mpeg-sdt.c \
    source/mpeg-system-header.c \
    source/mpeg-ts-dec.c \
    source/mpeg-ts-enc.c \
    source/mpeg-ts-h264.c \
    source/mpeg-ts-h265.c \
    source/mpeg-util.c



