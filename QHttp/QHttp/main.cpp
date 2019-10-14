#include <QCoreApplication>
#include "HttpCallback.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    new HttpCallback (&a);
    return a.exec();
}
