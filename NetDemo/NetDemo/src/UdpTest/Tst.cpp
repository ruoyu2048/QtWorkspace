#include "Tst.h"
#include <QDebug>
#include "DataStruct.h"

Tst::Tst(QObject *parent) : QObject(parent)
{
    udpCfg.bSimmulator = false;
    udpCfg.commType = UDP;
    udpCfg.strCommPara = "127.0.0.1:9998";
    udpCfg.strCommOther = "127.0.0.1:9999+0xA4";

    cumm.startCommunication(&udpCfg);
    connect(&cumm,SIGNAL(receivedDataPacket(CDataPacket*)),this,SLOT(test(CDataPacket*)));
}

void Tst::test(CDataPacket *dataPkt){
    if( NULL != dataPkt ){
        dataPkt->decodeData();
        CTTest datas;
        dataPkt->parseDataToStruct((uchar*)&datas,(int)sizeof(CTTest));
        qDebug()<<sizeof(CTTest)<<datas.cWorkMode<<datas.nShort<<datas.fData;

        CDataPacket dataFB;
        dataFB.msgHead = 0xAA;
        dataFB.msgEnd = 0xA5;
        dataFB.msgDst = 0xA4;
        dataFB.msgSrc = 0xA3;
        dataFB.msgType = 0x12;

        //某分机的某报文
        CTTest ctTest;
        ctTest.cCmd = 0x33;
        ctTest.cWorkMode =2;
        ctTest.nShort = 300;
        ctTest.fData = 600.2;

        dataFB.msgData.append((char*)&ctTest,sizeof(CTTest));
        dataFB.packPacketToEncodedBytes();
        cumm.sendDataPacket(&dataFB);
        qDebug()<<"反馈报文！";
    }
}
