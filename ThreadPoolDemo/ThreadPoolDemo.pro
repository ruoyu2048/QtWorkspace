TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    IWorkerPool.cpp \
    WorkerPool.cpp

HEADERS += \
    IWorkerPool.h \
    WorkerPool.h
