TARGET = librtp
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt
include(../libs.pri)
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

HEADERS += \
    include/rtcp-header.h \
    include/rtp.h \
    include/rtp-header.h \
    include/rtp-internal.h \
    include/rtp-member.h \
    include/rtp-member-list.h \
    include/rtp-packet.h \
    include/rtp-param.h \
    include/rtp-payload.h \
    include/rtp-profile.h \
    include/rtp-queue.h \
    include/rtp-util.h \
    payload/rtp-payload-helper.h \
    payload/rtp-payload-internal.h

SOURCES += \
    payload/rtp-h264-pack.c \
    payload/rtp-h264-unpack.c \
    payload/rtp-h265-pack.c \
    payload/rtp-h265-unpack.c \
    payload/rtp-mp4a-latm-pack.c \
    payload/rtp-mp4a-latm-unpack.c \
    payload/rtp-mp4v-es-pack.c \
    payload/rtp-mp4v-es-unpack.c \
    payload/rtp-mpeg1or2es-pack.c \
    payload/rtp-mpeg1or2es-unpack.c \
    payload/rtp-mpeg4-generic-pack.c \
    payload/rtp-mpeg4-generic-unpack.c \
    payload/rtp-pack.c \
    payload/rtp-payload.c \
    payload/rtp-payload-helper.c \
    payload/rtp-ts-pack.c \
    payload/rtp-ts-unpack.c \
    payload/rtp-unpack.c \
    payload/rtp-vp8-pack.c \
    payload/rtp-vp8-unpack.c \
    payload/rtp-vp9-pack.c \
    payload/rtp-vp9-unpack.c \
    source/rtcp.c \
    source/rtcp-app.c \
    source/rtcp-bye.c \
    source/rtcp-interval.c \
    source/rtcp-rr.c \
    source/rtcp-sdec.c \
    source/rtcp-sr.c \
    source/rtp.c \
    source/rtp-member.c \
    source/rtp-member-list.c \
    source/rtp-packet.c \
    source/rtp-profile.c \
    source/rtp-queue.c \
    source/rtp-ssrc.c \
    source/rtp-time.c

