#include <QCoreApplication>
#include "CTcpServer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CTcpServer* pTS = new CTcpServer();
    pTS->startListen("127.0.0.1",8080);

    return a.exec();
}
