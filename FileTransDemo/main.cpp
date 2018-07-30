#include "FileTransDemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FileTransDemo w;
    w.show();

    return a.exec();
}
