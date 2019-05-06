#include "MTFileTranslate.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MTFileTranslate w;
    w.show();

    return a.exec();
}
