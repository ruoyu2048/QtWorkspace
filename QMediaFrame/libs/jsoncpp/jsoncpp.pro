TARGET = jsoncpp
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt

include(../libs.pri)

HEADERS += \
    assertions.h \
    autolink.h \
    config.h \
    features.h \
    forwards.h \
    json.h \
    json_tool.h \
    reader.h \
    value.h \
    version.h \
    writer.h

SOURCES += \
    json_reader.cpp \
    json_value.cpp \
    json_writer.cpp
