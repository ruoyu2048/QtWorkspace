QT += core network
QT -= gui

TEMPLATE = app

SOURCES += \
    main.cpp \
    loggerobject.cpp \
    loggerobjectprio.cpp \
    loggerstatic.cpp

HEADERS  += \
    loggerobject.h \
    loggerobjectprio.h \
    loggerstatic.h

include(../apps.pri)

