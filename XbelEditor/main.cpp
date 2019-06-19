#include "XbelEditor.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file("./qss/treeView.qss");
    file.open(QFile::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    XbelEditor w("./files/jennifer.xbel");
    w.show();

    return a.exec();
}
