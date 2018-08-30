#include <QCoreApplication>
#include "../Communication/Communication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    Communication* pCom = new Communication();
//    comm.startTcpServer("127.0.0.1",9999);

    return a.exec();
}
