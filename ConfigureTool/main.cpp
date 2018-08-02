#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.setWindowTitle("配置管理工具");
    //mainWindow.showMaximized();
    mainWindow.show();
    return a.exec();
}
