#include <QCoreApplication>
#include "Communication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList strTS;
    /*|IP|PORT|*/
    strTS<<"127.0.0.1"<<"9999"<<"0x50";
    Communication* pCom = new Communication();
    pCom->startCommunication(TcpServer,strTS);
//    pCom->startCommunication(TcpClient,strTS);

    QStringList strTC;
    /*|SERVER_IP|SERVER_PORT|MSG_TYPE|DST_ID|.......*/
    strTC<<"127.0.0.1"<<"9999"<<"0x01"<<"0x30"<<"0x31";
    Communication* pCom2 = new Communication();
    pCom2->startCommunication(TcpClient,strTC);

    return a.exec();
}
