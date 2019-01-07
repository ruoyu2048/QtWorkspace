#include "XbelEditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XbelEditor w(":/jennifer.xbel");
    w.show();

    return a.exec();
}
