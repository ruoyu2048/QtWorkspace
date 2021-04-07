

TARGET = ToolKit
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib
CONFIG -= qt
include(../libs.pri)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Network/Buffer.h \
    Network/Socket.h \
    Network/sockutil.h \
    Network/TcpClient.h \
    Network/TcpServer.h \
    Network/TcpSession.h \
    Poller/EventPoller.h \
    Poller/Pipe.h \
    Poller/PipeWrap.h \
    Poller/SelectWrap.h \
    Poller/Timer.h \
    Thread/semaphore.h \
    Thread/TaskExecutor.h \
    Thread/TaskQueue.h \
    Thread/threadgroup.h \
    Thread/ThreadPool.h \
    Thread/WorkThreadPool.h \
    win32/getopt.h \
    win32/tailor.h \
    Util/base64.h \
    Util/CMD.h \
    Util/File.h \
    Util/function_traits.h \
    Util/List.h \
    Util/logger.h \
    Util/MD5.h \
    Util/mini.h \
    Util/NoticeCenter.h \
    Util/onceToken.h \
    Util/ResourcePool.h \
    Util/RingBuffer.h \
    Util/SHA1.h \
    Util/SqlConnection.h \
    Util/SqlPool.h \
    Util/SSLBox.h \
    Util/SSLUtil.h \
    Util/TimeTicker.h \
    Util/util.h \
    Util/uv_errno.h

SOURCES += \
    Network/Buffer.cpp \
    Network/Socket.cpp \
    Network/sockutil.cpp \
    Network/TcpClient.cpp \
    Network/TcpServer.cpp \
    Network/TcpSession.cpp \
    #Network/Socket_ios.mm \
    Poller/EventPoller.cpp \
    Poller/Pipe.cpp \
    Poller/PipeWrap.cpp \
    Poller/SelectWrap.cpp \
    Poller/Timer.cpp \
    Thread/WorkThreadPool.cpp \
    win32/getopt.c \
    Util/base64.cpp \
    Util/CMD.cpp \
    Util/File.cpp \
    Util/logger.cpp \
    Util/MD5.cpp \
    Util/mini.cpp \
    Util/NoticeCenter.cpp \
    Util/SHA1.cpp \
    Util/SqlPool.cpp \
    Util/SSLBox.cpp \
    Util/SSLUtil.cpp \
    Util/util.cpp \
    Util/uv_errno.cpp

#目标文件生成路径
#win32{
#    DESTDIR = $$OUT_PWD/../bin_windows

#}

#linux{
#    DESTDIR = $$OUT_PWD/../bin_linux

#}
#目标文件生成类型(调试版本或发布版本)
#CONFIG(debug, debug|release) {
#    TARGET = $$join(TARGET,,,d)
#}
#INCLUDEPATH += $$PWD/../jsoncpp
#DEPENDPATH += $$PWD/../jsoncpp
