#include <QApplication>
#include "PluginMoinitorDisplay.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PluginMoinitorDisplay mainWindow;
    mainWindow.setWindowTitle("配置管理工具");
    //mainWindow.showMaximized();
    mainWindow.show();
    return a.exec();
}
