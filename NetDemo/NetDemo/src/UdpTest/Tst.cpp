#include "Tst.h"
#include <QDebug>
#include "DataStruct.h"

Tst::Tst(QObject *parent) : QObject(parent)
{
    udpCfg.bSimmulator = false;
    udpCfg.commType = UDP;
    udpCfg.strCommPara = "127.0.0.1:9998";
    udpCfg.strCommOther = "127.0.0.1:9999+0x11";
    cumm.startCommunication(&udpCfg);
    connect(&cumm,SIGNAL(receivedDataPacket(CDataPacket*)),this,SLOT(test(CDataPacket*)));
}

void Tst::test(CDataPacket *dataPkt){
    if( NULL != dataPkt ){
        dataPkt->encoddeBytesToPacket();
        qDebug()<<dataPkt->msgType;
        CTTest datas;
        memcpy(&datas,dataPkt->msgData.data(),sizeof(CTTest));
        qDebug()<<datas.cWorkMode<<datas.nShort<<datas.fData;
    }
}
