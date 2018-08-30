#include <QCoreApplication>
#include "Communication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Communication* pCom = new Communication();
    pCom->startTcpServer("127.0.0.1",9999);
    //pCom->startTcpClient("127.0.0.1",9999);

    return a.exec();
}
