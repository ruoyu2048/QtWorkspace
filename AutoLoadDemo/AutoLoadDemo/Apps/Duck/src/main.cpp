#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Microsoft Yahei", 10));
    QApplication::setStyle("Fusion");
    MainWindow w;
    w.show();

    return a.exec();
}
