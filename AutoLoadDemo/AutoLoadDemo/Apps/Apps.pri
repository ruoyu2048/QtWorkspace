
DESTDIR=../../


INCLUDEPATH += $$PWD/../Libs/PluginManager
DEPENDPATH  += $$PWD/../Libs/PluginManager

LIBS += $$OUT_PWD/../../libdir/libPluginManager.a
#unix{
#    LIBS += $$OUT_PWD/../../libdir/PluginManager.a

#}
#else:win32{
#    LIBS += $$OUT_PWD/../../libdir/PluginManager.lib

#}
