QT += core
QT -= gui

CONFIG += c++11
#设置输出路径
win32{
    DESTDIR=$${OUT_PWD}/bin.windows
}
linux{
    DESTDIR=$${OUT_PWD}/bin.linux
}

CONFIG(debug,debug|release){
    TARGET = Log4CplusDemoD
}
else{
    TARGET = Log4CplusDemo
}
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += $$PWD/include


CONFIG(debug,debug|release){
#    #<1>MINGW_MSVC_2017_x64(均不可用)
#    LIBS += $$PWD/libs/windows/mingw_x64/log4cplusUD.dll
    #<2>MINGW_x86(MinGW可用)
    LIBS += $$PWD/libs/windows/mingw_x86/liblog4cplusUD.dll
#    #<3>MSVC_2017_x64(均不可用)
#    LIBS += $$PWD/libs/windows/msvc_x64/log4cplusUD.dll
#    #<4>MSVC_2017_x86(均不可用)
#    LIBS += $$PWD/libs/windows/msvc_x86/log4cplusUD.dll
}
else{
#    #<1>MINGW_MSVC_2017_x64(均不可用)
#    LIBS += $$PWD/libs/windows/mingw_x64/log4cplusU.dll
    #<2>MINGW_x86(MinGW可用)
    LIBS += $$PWD/libs/windows/mingw_x86/liblog4cplusU.dll
#    #<3>MSVC_2017_x64(均不可用)
#    LIBS += $$PWD/libs/windows/msvc_x64/log4cplusU.dll
#    #<4>MSVC_2017_x86(均不可用)
#    LIBS += $$PWD/libs/windows/msvc_x86/log4cplusU.dll
}

