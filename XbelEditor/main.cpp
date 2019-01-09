#include "XbelEditor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XbelEditor w("./config/jennifer.xml");
    w.show();

    return a.exec();
}
