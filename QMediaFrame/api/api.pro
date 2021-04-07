
TARGET = api
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt

#目标文件生成路径
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../bind/api
}else{
    DESTDIR = $$OUT_PWD/../bin/api
}

#目标文件生成类型(调试版本或发布版本)
build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

HEADERS += \
    include/mk_common.h \
    include/mk_events.h \
    include/mk_events_objects.h \
    include/mk_httpclient.h \
    include/mk_media.h \
    include/mk_mediakit.h \
    include/mk_player.h \
    include/mk_proxyplayer.h \
    include/mk_pusher.h \
    include/mk_recorder.h \
    include/mk_tcp.h \
    include/mk_thread.h \
    include/mk_util.h \
    source/mk_tcp_private.h

SOURCES += \
    source/mk_common.cpp \
    source/mk_events.cpp \
    source/mk_events_objects.cpp \
    source/mk_httpclient.cpp \
    source/mk_media.cpp \
    source/mk_player.cpp \
    source/mk_proxyplayer.cpp \
    source/mk_pusher.cpp \
    source/mk_recorder.cpp \
    source/mk_tcp.cpp \
    source/mk_thread.cpp \
    source/mk_util.cpp


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

INCLUDEPATH += $$PWD/../libs/ToolKit
DEPENDPATH += $$PWD/../libs/ToolKit

INCLUDEPATH += $$PWD/../libs/MediaKit
DEPENDPATH += $$PWD/../libs/MediaKit

INCLUDEPATH += $$PWD/../libs/libmpeg/include
DEPENDPATH += $$PWD/../libs/libmpeg/include

INCLUDEPATH += $$PWD/../libs/libmov/include
DEPENDPATH += $$PWD/../libs/libmov/include

INCLUDEPATH += $$PWD/../libs/libflv/include
DEPENDPATH += $$PWD/../libs/libflv/include

INCLUDEPATH += $$PWD/../libs/librtp/include
DEPENDPATH += $$PWD/../libs/librtp/include

CONFIG(debug, debug|release){
    LIBS += -L$$OUT_PWD/../bind/kits/ -lToolKitd
    LIBS += -L$$OUT_PWD/../bind/kits/ -lMediaKitd
    LIBS += -L$$OUT_PWD/../bind/libs/ -llibmpegd
    LIBS += -L$$OUT_PWD/../bind/libs/ -llibmovd
    LIBS += -L$$OUT_PWD/../bind/libs/ -llibflvd
    LIBS += -L$$OUT_PWD/../bind/libs/ -llibrtpd
    LIBS += -L$$OUT_PWD/../bind/libs/ -ljsoncppd
}else{
    LIBS += -L$$OUT_PWD/../bin/kits/ -lToolKit
    LIBS += -L$$OUT_PWD/../bin/kits/ -lMediaKit
    LIBS += -L$$OUT_PWD/../bin/libs/ -llibmpeg
    LIBS += -L$$OUT_PWD/../bin/libs/ -llibmov
    LIBS += -L$$OUT_PWD/../bin/libs/ -llibflv
    LIBS += -L$$OUT_PWD/../bin/libs/ -llibrtp
    LIBS += -L$$OUT_PWD/../bin/libs/ -ljsoncpp
}

DEFINES+=WIN32
DEFINES+=_WINDOWS
#DEFINES+=DBUG_OFF
DEFINES+=ENABLE_HLS
DEFINES+=ENABLE_MP4RECORD
DEFINES+=ENABLE_RTPPROXY
DEFINES+=WIN32_LEAN_AND_MEAN
DEFINES+=MP4V2_NO_STDINT_DEFS
DEFINES+=OS_WINDOWS
DEFINES+=MediaKitApi_EXPORTS
DEFINES+=mk_api_EXPORTS



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



