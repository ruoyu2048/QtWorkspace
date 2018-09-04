#include <QCoreApplication>
#include "Communication.h"
#include "CommunicationCfg.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CommunicationCfg cfgTS;
    cfgTS.parseCommCfgFile(":/config/connection_cfg _server.json");
    Communication* pCommTS = new Communication();
    pCommTS->startCommunication(&cfgTS);

    CommunicationCfg cfgTC;
    cfgTC.parseCommCfgFile(":/config/connection_cfg_client.json");
    Communication* pCommTC = new Communication();
    pCommTC->startCommunication(&cfgTC);

    return a.exec();
}
