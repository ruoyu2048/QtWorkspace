
DESTDIR=../../


INCLUDEPATH += $$PWD/../Libs/PluginManager
DEPENDPATH  += $$PWD/../Libs/PluginManager

LIBS += $$OUT_PWD/../../LibDir/PluginManager.lib
#unix{
#    LIBS += $$OUT_PWD/../../LibDir/PluginManager.a

#}
#else:win32{
#    LIBS += $$OUT_PWD/../../LibDir/PluginManager.lib

#}
