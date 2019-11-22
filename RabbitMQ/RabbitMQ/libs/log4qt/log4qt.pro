QT += core xml network
QT -= gui

include(log4qt.pri)

CONFIG +=  hide_symbols

TARGET = log4qt
TEMPLATE = lib

#=============================================================#
#引用libs.pri文件(include(../libs.pri))时，务必将引用语句放在*.pro文
#件中TARGET变量的下方，否目标文件生成类型目标文件生成类型(调试版本或发布版
#本)的条件编译将不起作用，具体位置按实际情况调整。
include(../libs.pri)
#=============================================================#

# .. is needed for msvc since it is treating '.' as the directory of the current file
# and not the directory where the compiled source is found
INCLUDEPATH += .. .

DEFINES += NOMINMAX QT_DEPRECATED_WARNINGS QT_NO_CAST_FROM_BYTEARRAY QT_USE_QSTRINGBUILDER
DEFINES += LOG4QT_LIBRARY

#target.path = $$INSTALL_PREFIX/lib$$LIB_SUFFIX
#INSTALLS = target

#header_base.files = $$HEADERS_BASE
#header_base.path = $$INSTALL_PREFIX/include/log4qt
#INSTALLS += header_base
#header_helpers.files = $$HEADERS_HELPERS
#header_helpers.path = $$INSTALL_PREFIX/include/log4qt/helpers
#INSTALLS += header_helpers
#header_spi.files = $$HEADERS_SPI
#header_spi.path = $$INSTALL_PREFIX/include/log4qt/spi
#INSTALLS += header_spi
#header_varia.files = $$HEADERS_VARIA
#header_varia.path = $$INSTALL_PREFIX/include/log4qt/varia
#INSTALLS += header_varia

