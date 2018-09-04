#include <QCoreApplication>
#include "Communication.h"
#include "CCommCfg.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CCommCfg cfgTS;
    cfgTS.parseCommCfgFile(":/config/connection_cfg _server.json");
    Communication* pCommTS = new Communication();
    pCommTS->startCommunication(&cfgTS);

    CCommCfg cfgTC;
    cfgTC.parseCommCfgFile(":/config/connection_cfg_client.json");
    Communication* pCommTC = new Communication();
    pCommTC->startCommunication(&cfgTC);

    return a.exec();
}
