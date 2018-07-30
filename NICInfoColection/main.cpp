#include "NICInfoColection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CNICInfoColection w;
    w.show();

    return a.exec();
}
