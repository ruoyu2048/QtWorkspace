TARGET = libmov
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib

CONFIG -= qt
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

include(../libs.pri)

HEADERS += \
    include/fmp4-writer.h \
    include/mov-atom.h \
    include/mov-box.h \
    include/mov-buffer.h \
    include/mov-format.h \
    include/mov-reader.h \
    include/mov-writer.h \
    source/mov-internal.h \
    source/mov-ioutil.h

SOURCES += \
    source/fmp4-writer.c \
    source/mov-avc1.c \
    source/mov-dinf.c \
    source/mov-elst.c \
    source/mov-esds.c \
    source/mov-ftyp.c \
    source/mov-hdlr.c \
    source/mov-hvcc.c \
    source/mov-iods.c \
    source/mov-leva.c \
    source/mov-mdhd.c \
    source/mov-mehd.c \
    source/mov-mfhd.c \
    source/mov-minf.c \
    source/mov-mvhd.c \
    source/mov-opus.c \
    source/mov-reader.c \
    source/mov-sidx.c \
    source/mov-stco.c \
    source/mov-stsc.c \
    source/mov-stsd.c \
    source/mov-stss.c \
    source/mov-stsz.c \
    source/mov-stts.c \
    source/mov-tag.c \
    source/mov-tfdt.c \
    source/mov-tfhd.c \
    source/mov-tfra.c \
    source/mov-tkhd.c \
    source/mov-track.c \
    source/mov-trex.c \
    source/mov-trun.c \
    source/mov-tx3g.c \
    source/mov-writer.c

