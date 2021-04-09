QT          += core network
QT          -= gui
CONFIG      += console
osx:CONFIG  -= app_bundle

TARGET       = authorizationManagementService

TEMPLATE     = app


include(../authorizes.pri)

HEADERS += \
    ClientHandler.h \
    AuthorizationManagementService.h \
    AuthCodeJson.h \
    base64.h \
    Crypto.h

SOURCES += \
    main.cpp \
    ClientHandler.cpp \
    AuthorizationManagementService.cpp \
    AuthCodeJson.cpp \
    base64.cpp \
    Crypto.cpp
