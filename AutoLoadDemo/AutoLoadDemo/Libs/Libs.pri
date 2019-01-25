DESTDIR=../../LibDir


INCLUDEPATH += $$PWD/PluginManager
DEPENDPATH += $$PWD/PluginManager

unix{
    LIBS += $$OUT_PWD/../../libdir/PluginManager.a

}
else:win32{
    LIBS += $$OUT_PWD/../../libdir/PluginManager.lib

}
