#include <QCoreApplication>
#include "Communication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList strTS;
    strTS<<"127.0.0.1"<<"8080";
    Communication* pCom = new Communication();
    pCom->startCommunication(TcpServer,strTS);

    Communication* pCom2 = new Communication();
    pCom2->startCommunication(TcpClient,strTS);

    return a.exec();
}
