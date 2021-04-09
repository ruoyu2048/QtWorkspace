QT          += core network
QT          -= gui
CONFIG      += console
osx:CONFIG  -= app_bundle

TARGET       = authorizedServices

TEMPLATE     = app


include(../apps.pri)

HEADERS += \
    AppCodeJson.h \
    ClientHandler.h \
    DeviceInfo.h \
    AuthorizedServices.h

SOURCES += \
    AppCodeJson.cpp \
    DeviceInfo.cpp \
    AuthorizedServices.cpp \
    main.cpp \
    ClientHandler.cpp
