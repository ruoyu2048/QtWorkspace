#include "ThreadDemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ThreadDemo w;
    w.show();

    return a.exec();
}
