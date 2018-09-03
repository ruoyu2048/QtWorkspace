#include <QCoreApplication>
#include "Communication.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList strTS;
    /*|IP|PORT|*/
    strTS<<"127.0.0.1"<<"9999";
    Communication* pCom = new Communication();
    pCom->startCommunication(TcpServer,strTS);

    QStringList strTC1;
    /*|SERVER_IP|SERVER_PORT|MSG_TYPE|DST_ID|.......*/
    strTC1<<"127.0.0.1"<<"9999"<<"0x10"<<"0x30";//0x31
    Communication* pCom1 = new Communication();
    pCom1->startCommunication(TcpClient,strTC1);

    QStringList strTC2;
    /*|SERVER_IP|SERVER_PORT|MSG_TYPE|DST_ID|.......*/
    strTC2<<"127.0.0.1"<<"9999"<<"0x20"<<"0x32";//0x33
    Communication* pCom2 = new Communication();
    pCom2->startCommunication(TcpClient,strTC2);

    return a.exec();
}
