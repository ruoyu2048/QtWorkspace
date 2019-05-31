#include "dialog.h"
#include "ui_dialog.h"

#include <QDebug>

#include "Communication.h"

#include "CDataTranscode.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
//    CommunicationCfg cfg;
//    //cfg.CommuType=CommunicationType::TcpServer;
//    cfg.CommuType=CommunicationType::TcpClient;
//    cfg.strCommuId=1;
//    cfg.strCommuPara="127.0.0.1:8080";
//    mm.startDataOperation(cfg);

    QByteArray msgdata="ABCDEFGHIJKLMN";
    QByteArray ary;
    ary.append(0xAA);
    ary.append(0x01);
    ary.append(0x02);
    ary.append(0x03);
    short nnn=msgdata.size();
    ary.append((char*)&nnn,sizeof(nnn));
    ary.append(msgdata);
    ary.append(0xA5);
    qDebug()<<ary;
    CDataTranscode sss;
    sss.encodeData(ary);
    sss.decodeData(sss.getEncodeDataAry());
    qDebug()<<sss.getDecodeDataAry();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onTest(QByteArray sss)
{
    qDebug()<<sss;
}
