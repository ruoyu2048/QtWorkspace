#include <QCoreApplication>
#include "Communication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList strTS;
    strTS<<"192.168.8.107"<<"8080";
    Communication* pCom = new Communication();
    pCom->startCommunication(TcpServer,strTS);
    pCom->startCommunication(TcpClient,strTS);

    return a.exec();
}
