#include "XbelEditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XbelEditor w(":/jennifer.xml");
    w.show();

    return a.exec();
}
