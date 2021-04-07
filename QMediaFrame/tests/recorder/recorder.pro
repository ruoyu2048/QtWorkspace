TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


include(../tests.pri)

SOURCES += \
        main.cpp

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

INCLUDEPATH += $$PWD/../../libs/ToolKit
DEPENDPATH += $$PWD/../../libs/ToolKit

INCLUDEPATH += $$PWD/../../libs/MediaKit
DEPENDPATH += $$PWD/../../libs/MediaKit

INCLUDEPATH += $$PWD/../../libs/libmpeg/include
DEPENDPATH += $$PWD/../../libs/libmpeg/include

INCLUDEPATH += $$PWD/../../libs/libmov/include
DEPENDPATH += $$PWD/../../libs/libmov/include

INCLUDEPATH += $$PWD/../../libs/libflv/include
DEPENDPATH += $$PWD/../../libs/libflv/include

INCLUDEPATH += $$PWD/../../libs/librtp/include
DEPENDPATH += $$PWD/../../libs/librtp/include

CONFIG(debug, debug|release){
    LIBS += -L$$OUT_PWD/../../bind/kits/ -lToolKitd
    LIBS += -L$$OUT_PWD/../../bind/kits/ -lMediaKitd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibmpegd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibmovd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibflvd
    LIBS += -L$$OUT_PWD/../../bind/libs/ -llibrtpd
}else{
    LIBS += -L$$OUT_PWD/../../bin/kits/ -lToolKit
    LIBS += -L$$OUT_PWD/../../bin/kits/ -lMediaKit
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibmpeg
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibmov
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibflv
    LIBS += -L$$OUT_PWD/../../bin/libs/ -llibrtp
}


