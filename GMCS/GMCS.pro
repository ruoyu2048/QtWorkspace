#-------------------------------------------------
#
# Project created by QtCreator 2016-10-09T15:47:39
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GMCS
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    TreeItem.cpp \
    TreeModel.cpp \
    DataSet.cpp \
    TreeView.cpp \
    SortFilterProxyModel.cpp \
    FilterDlg.cpp \
    NetworkModule.cpp \
    NetworkCheck.cpp \
    SendFileDlg.cpp

HEADERS  += MainWindow.h \
    DataStruct.h \
    GlobalDef.h \
    MacroDef.h \
    TreeItem.h \
    TreeModel.h \
    DataSet.h \
    TreeView.h \
    SortFilterProxyModel.h \
    FilterDlg.h \
    NetworkModule.h \
    NetworkCheck.h \
    SendFileDlg.h

FORMS    += MainWindow.ui

RESOURCES += \
    res.qrc

RC_FILE = GMCS.rc
