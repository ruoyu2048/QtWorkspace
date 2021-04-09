QT          += core network
QT          -= gui
CONFIG      += console
osx:CONFIG  -= app_bundle

TARGET       = authorizedServices

TEMPLATE     = app


include(../authorizes.pri)

DEFINES *= QHTTP_HAS_CLIENT

HEADERS += \
    AppCodeJson.h \
    ClientHandler.h \
    DeviceInfo.h \
    AuthorizedServices.h \
    base64.h \
    Crypto.h \
    AuthorizedClient.h

SOURCES += \
    AppCodeJson.cpp \
    DeviceInfo.cpp \
    AuthorizedServices.cpp \
    main.cpp \
    ClientHandler.cpp \
    base64.cpp \
    Crypto.cpp \
    AuthorizedClient.cpp
