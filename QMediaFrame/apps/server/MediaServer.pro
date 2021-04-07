TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


HEADERS += \
    FFmpegSource.h \
    Process.h \
    System.h \
    WebApi.h \
    WebHook.h

SOURCES += \
    FFmpegSource.cpp \
    main.cpp \
    Process.cpp \
    System.cpp \
    WebApi.cpp \
    WebHook.cpp


DEFINES+=WIN32
DEFINES+=_WINDOWS
#DEFINES+=DBUG_OFF
DEFINES+=ENABLE_HLS
DEFINES+=ENABLE_MP4RECORD
DEFINES+=ENABLE_RTPPROXY
DEFINES+=WIN32_LEAN_AND_MEAN
DEFINES+=MP4V2_NO_STDINT_DEFS
DEFINES+=OS_WINDOWS


LIBS+=WS2_32.lib
LIBS+=Iphlpapi.lib
LIBS+=shlwapi.lib
LIBS+=kernel32.lib
LIBS+=user32.lib
LIBS+=gdi32.lib
LIBS+=winspool.lib
LIBS+=shell32.lib
LIBS+=ole32.lib
LIBS+=oleaut32.lib
LIBS+=uuid.lib
LIBS+=comdlg32.lib
LIBS+=advapi32.lib


INCLUDEPATH += $$PWD/../../libs/libmpeg/include
DEPENDPATH += $$PWD/../../libs/libmpeg/include
LIBS += -L$$OUT_PWD/../../libs/libmpeg/debug/ -llibmpeg



INCLUDEPATH += $$PWD/../../libs/libmov/include
DEPENDPATH += $$PWD/../../libs/libmov/include
LIBS += -L$$OUT_PWD/../../libs/libmov/debug/ -llibmov


INCLUDEPATH += $$PWD/../../libs/libflv/include
DEPENDPATH += $$PWD/../../libs/libflv/include
LIBS += -L$$OUT_PWD/../../libs/libflv/debug/ -llibflv


INCLUDEPATH += $$PWD/../../libs/librtp/include
DEPENDPATH += $$PWD/../../libs/librtp/include
LIBS += -L$$OUT_PWD/../../libs/librtp/debug/ -llibrtp

INCLUDEPATH += $$PWD/../../libs/jsoncpp/
DEPENDPATH += $$PWD/../../libs/jsoncpp/
LIBS += -L$$OUT_PWD/../../libs/jsoncpp/debug/ -ljsoncpp


INCLUDEPATH += $$PWD/../../libs
DEPENDPATH += $$PWD/../../libs

INCLUDEPATH += $$PWD/../../libs/tools
DEPENDPATH += $$PWD/../../libs/tools
LIBS += -L$$OUT_PWD/../../libs/tools/debug/ -ltools


INCLUDEPATH += $$PWD/../../libs/MediaKit
DEPENDPATH += $$PWD/../../libs/MediaKit
LIBS += -L$$OUT_PWD/../../libs/MediaKit/debug/ -lMediaKit

