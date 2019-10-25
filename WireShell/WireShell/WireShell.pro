#-------------------------------------------------
#
# Project created by QtCreator 2019-10-24T14:40:29
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#目标文件生成路径
DESTDIR = $$OUT_PWD/bin

TARGET = WireShell
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        WireShell.cpp \
    LoginDlg.cpp \
    RunTest.cpp \
    SQLite.cpp \
    TestDisplay.cpp

HEADERS += \
        WireShell.h \
    LoginDlg.h \
    RunTest.h \
    SQLite.h \
    TestDisplay.h

FORMS += \
        WireShell.ui \
    LoginDlg.ui \
    RunTest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

##================================================================#
##依赖库路径宏定义
#WIRE_API_PATH = $${PWD}/WireAPI
##添加依赖库路径
#INCLUDEPATH += $${WIRE_API_PATH}
##================================================================#
##////////////////////////////////////////////////////////////////#
##================================================================#

##添加动态库路径
#LIBS += -L$${DESTDIR} -lWireAPI
##================================================================#

RESOURCES += \
    srs.qrc



