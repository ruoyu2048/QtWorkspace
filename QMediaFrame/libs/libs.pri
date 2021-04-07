DEFINES+=WIN32
DEFINES+=_WINDOWS
#DEFINES+=DBUG_OFF
DEFINES+=ENABLE_HLS
DEFINES+=ENABLE_MP4RECORD
DEFINES+=ENABLE_RTPPROXY
DEFINES+=WIN32_LEAN_AND_MEAN
DEFINES+=MP4V2_NO_STDINT_DEFS
#DEFINES+=ENABLE_MP4V2
DEFINES+=OS_WINDOWS

#=============================================================#
#引用libs.pri文件(include(../libs.pri))时，务必将引用语句放在*.pro文
#件中TARGET变量的下方，否目标文件生成类型目标文件生成类型(调试版本或发布版
#本)的条件编译将不起作用。
#=============================================================#
#目标文件生成路径
CONFIG(debug, debug|release){
    DESTDIR = $$OUT_PWD/../../bind/libs
}else{
    DESTDIR = $$OUT_PWD/../../bin/libs
}

#目标文件生成类型(调试版本或发布版本)
build_pass:CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}
