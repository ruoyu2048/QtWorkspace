#include "PluginMacroConfig.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("./config/qss/treeView.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    PluginMacroConfig w;
    w.showNormal();

    return a.exec();
}
