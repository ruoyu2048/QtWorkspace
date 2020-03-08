QT -= gui

QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


PROJECT_ROOT = $$OUT_PWD/..

CONFIG(debug, debug|release) {
        DIR_OUTPUT = $$PROJECT_ROOT/bind
        DEFINES += QHTTP_REQUESTS_DEBUG
#        LIBS += -L$${DIR_OUTPUT} -llibHttp
}
CONFIG(release, debug|release) {
        DIR_OUTPUT = $$PROJECT_ROOT/bin
}
!contains(BUILD_CONFIG, THIRD_PARTY_BUILD) {
    DESTDIR = $$DIR_OUTPUT
}

INCLUDEPATH += $${PWD}/../libs/libHttp
LIBS += -L$$OUT_PWD/../bind/ -llibHttp

