#include <QtCore/QCoreApplication>
#include "AuthorizationManagementService.h"
#include <QtDebug>
#include "AuthCodeJson.h"
int main(int argc, char ** argv) {
    QCoreApplication app(argc, argv);
    AuthorizationManagementService ams(&app);
    ams.startListen();

//    AuthCodeJson acj;
//    acj.rsaEncode();

    app.exec();
}
