#-------------------------------------------------
#
# Project created by QtCreator 2019-09-25T11:32:47
#
#-------------------------------------------------

QT       -= gui
QT       += core network

TARGET = qftp
TEMPLATE = lib
#CONFIG -= staticlib

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        qftp.cpp \
        qurlinfo.cpp

HEADERS += \
        qftp.h \
        qurlinfo.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

#=============================================================#
#引用libs.pri文件(include(../libs.pri))时，务必将引用语句放在*.pro文
#件中TARGET变量的下方，否目标文件生成类型目标文件生成类型(调试版本或发布版
#本)的条件编译将不起作用，具体位置按实际情况调整。
include(../libs.pri)
#=============================================================#
