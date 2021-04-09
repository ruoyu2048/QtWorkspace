#include <QtCore/QCoreApplication>
#include "DeviceInfo.h"
#include "AppCodeJson.h"
#include "AuthorizedServices.h"
#include <QtDebug>

#include "AuthorizedClient.h"

int main(int argc, char ** argv) {
    QCoreApplication app(argc, argv);
    //解决windows下汉字输乱码问题
    system("chcp 65001");

    AuthorizedClient ac(&app);
    QString str_url("192.168.2.107:9080/empower/authorization_code/get_by_string");
    ac.start(str_url);

    AuthorizedServices es(&app);
    es.startListen("192.168.2.114",8080);

    app.exec();
}
