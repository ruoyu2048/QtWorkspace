TEMPLATE = app
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#目标文件生成路径
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../bind
}else{
    DESTDIR = $$OUT_PWD/../../bin
}

#目标文件生成类型(调试版本或发布版本)
build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}


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

INCLUDEPATH += $$PWD/../../libs/libmov/include
DEPENDPATH += $$PWD/../../libs/libmov/include

INCLUDEPATH += $$PWD/../../libs/libflv/include
DEPENDPATH += $$PWD/../../libs/libflv/include

INCLUDEPATH += $$PWD/../../libs/librtp/include
DEPENDPATH += $$PWD/../../libs/librtp/include

#INCLUDEPATH += $$PWD/../../libs/jsoncpp/
#DEPENDPATH += $$PWD/../../libs/jsoncpp/

INCLUDEPATH += $$PWD/../../libs/
DEPENDPATH += $$PWD/../../libs/

INCLUDEPATH += $$PWD/../../libs/ToolKit
DEPENDPATH += $$PWD/../../libs/ToolKit

INCLUDEPATH += $$PWD/../../libs/MediaKit
DEPENDPATH += $$PWD/../../libs/MediaKit

CONFIG(debug, debug|release){
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibmpegd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibmovd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibflvd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibrtpd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -ljsoncppd
    LIBS += -L$$OUT_PWD/../../bind/kits/ -lToolKitd
    LIBS += -L$$OUT_PWD/../../bind/kits/ -lMediaKitd
}else{
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibmpeg
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibmov
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibflv
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibrtp
    LIBS += -L$$OUT_PWD/../../bin/libs/ -ljsoncpp
    LIBS += -L$$OUT_PWD/../../bin/kits/ -lToolKit
    LIBS += -L$$OUT_PWD/../../bin/kits/ -lMediaKit
}
