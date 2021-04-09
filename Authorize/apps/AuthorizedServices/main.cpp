#include <QtCore/QCoreApplication>
#include "DeviceInfo.h"
#include "AppCodeJson.h"
#include "AuthorizedServices.h"
#include <QtDebug>

int main(int argc, char ** argv) {
    QCoreApplication app(argc, argv);
//    AuthorizedServices es(&app);
//    es.startListen();

//    DeviceInfo di;
//    qDebug()<<di.sysName();
//    qDebug()<<di.machineName();
//    qDebug()<<di.ip();
//    qDebug()<<di.mac();
//    qDebug()<<di.baseboardNum();
//    qDebug()<<di.harddiskNum();

    AppCodeJson acj;
    acj.rsaEncode();
    acj.rsaDecode();
    app.exec();
}
