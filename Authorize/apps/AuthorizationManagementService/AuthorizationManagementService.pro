QT          += core network
QT          -= gui
CONFIG      += console
osx:CONFIG  -= app_bundle

TARGET       = authorizationManagementService

TEMPLATE     = app


include(../apps.pri)

HEADERS += \
    ClientHandler.h \
    AuthorizationManagementService.h \
    AuthCodeJson.h

SOURCES += \
    main.cpp \
    ClientHandler.cpp \
    AuthorizationManagementService.cpp \
    AuthCodeJson.cpp
