#include "PluginMacroConfig.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PluginMacroConfig w;
    w.showNormal();

    return a.exec();
}
