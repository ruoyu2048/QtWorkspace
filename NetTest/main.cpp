#include <QCoreApplication>

//#include "CTcpClient.h"
//#include "CTcpServer.h"
//#include "CUdp.h"

#include "./Libs/CTcpClient/CTcpClient.h"
#include "./Libs/CTcpServer/CTcpServer.h"
#include "./Libs/CUdp/CUdp.h"

#include <QLibrary>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    CTcpServer* m_pTS = new CTcpServer();
//    m_pTS->StartListen("127.0.0.1",9999);

    CTcpClient* m_pTC0 = new CTcpClient();
    m_pTC0->ConnectToHost("127.0.0.1",9999);

    //CTcpClient* m_pTC1 = new CTcpClient();
    //m_pTC1->ConnectToHost(QHostAddress::LocalHost,10086);

//    CUdp* m_pUDPServer = new CUdp();
//    m_pUDPServer->Bind(QHostAddress::LocalHost,10001);

//    CUdp* m_pUDPClient = new CUdp();
//    m_pUDPClient->Bind(QHostAddress::LocalHost,10002);
//    m_pUDPClient->StartTest();

    return a.exec();
}
