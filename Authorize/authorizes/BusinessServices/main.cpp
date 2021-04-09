#include <QCoreApplication>

#include "BusinessServices.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BusinessServices bs;
    QString str_url("localhost:8080/business/verify");
    bs.start(str_url);
//    QByteArray ss=bs.getRequestJsonValue();
//    QByteArray gg=bs.getEncryptedJsonData(ss);
//    QByteArray cc=bs.getDecryptedJsonData(gg);
//    qDebug()<<ss.toStdString().data();
//    qDebug()<<cc.toStdString().data();

    return a.exec();
}
