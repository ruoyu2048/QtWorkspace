#include <QCoreApplication>
#include "Communication.h"
#include "CommunicationCfg.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    CommunicationCfg cfgTS;
//    cfgTS.parseCommCfgFile(":/config/connection_cfg_server.json");
//    Communication* pCommTS = new Communication();
//    pCommTS->startRecoder(false);
//    pCommTS->startCommunication(&cfgTS);

    CommunicationCfg cfgTCSim;
    cfgTCSim.parseCommCfgFile(":/config/connection_cfg_client.json");
    Communication* pCommTCSim = new Communication();
    pCommTCSim->startCommunication(&cfgTCSim);

//    CommunicationCfg cfgTCTest;
//    cfgTCTest.bSimmulator = false;
//    cfgTCTest.commType = TcpClient;
//    cfgTCTest.strCommPara = "127.0.0.1:9999";
//    cfgTCTest.strCommOther = "0x40";
//    //cfgTCSim.parseCommCfgFile(":/config/connection_cfg_client.json");
//    Communication* pCommTCTest = new Communication();
//    pCommTCTest->startCommunication(&cfgTCTest);

    return a.exec();
}
