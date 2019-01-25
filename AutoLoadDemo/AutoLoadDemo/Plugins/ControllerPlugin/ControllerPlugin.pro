#------------------------------------------------- 
# 
# Project created by NewProject 
# 
#------------------------------------------------- 


QT       += widgets 


TARGET = ControllerPlugin
TEMPLATE = lib 


DEFINES += CONTROLLERPLUGIN_LIBRARY 

SOURCES += controllerplugin.cpp\
    widget.cpp


HEADERS += controllerplugin.h\
        controllerplugin_global.h \
    widget.h


FORMS += \
    widget.ui



RESOURCES += \
    resource.qrc

include (../Plugins.pri)
