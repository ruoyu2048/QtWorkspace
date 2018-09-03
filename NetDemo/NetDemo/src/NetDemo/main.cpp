#include <QCoreApplication>
#include "Communication.h"
#include "CCommCfg.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CCommCfg cfg;
    cfg.commType = TcpServer;
    cfg.strCommPara = "127.0.0.1:9999";
    Communication* pCom = new Communication();
    pCom->startCommunication(&cfg);

//    QStringList strTC1;
//    /*|SERVER_IP|SERVER_PORT|MSG_TYPE(订阅的消息类型)|DST_ID|.......*/
//    strTC1<<"127.0.0.1"<<"9999"<<"0x40"<<"0x30";
//    Communication* pCom1 = new Communication();
//    pCom1->startCommunication(TcpClient,strTC1);

//    QStringList strTC2;
//    /*|SERVER_IP|SERVER_PORT|MSG_TYPE(订阅的消息类型)|DST_ID|.......*/
//    strTC2<<"127.0.0.1"<<"9999"<<"0x20"<<"0x32";//0x33
//    Communication* pCom2 = new Communication();
//    pCom2->startCommunication(TcpClient,strTC2);

    return a.exec();
}
