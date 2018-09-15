#include "CDataDeal.h"
#include "DataStruct.h"
#include "CDataPacket.h"
#include <QDebug>

CDataDeal::CDataDeal(QObject *parent) : QObject(parent)
{
    udpCfg.bSimmulator = false;
    udpCfg.commType = UDP;
    udpCfg.strCommPara = "127.0.0.1:9999";
    udpCfg.strCommOther = "127.0.0.1:9998+0xA2";
    comm.startCommunication(&udpCfg);
}

void CDataDeal::packMessage(uchar cCmdId, QStringList attrList){
    CDataPacket dataPkt;
    dataPkt.msgHead = 0xAA;
    dataPkt.msgEnd = 0xA5;
    switch (cCmdId) {
    case 0x20:{
        CTTest ctTest;
        ctTest.cCmd = cCmdId;
        ctTest.cWorkMode = attrList.at(0).toInt();
        ctTest.nShort = attrList.at(1).toShort();
        ctTest.fData = attrList.at(2).toFloat();
        dataPkt.msgDst = 0xA2;
        dataPkt.msgSrc = 0xA3;
        dataPkt.msgType = 0x11;
        dataPkt.msgData.append((char*)&ctTest,sizeof(CTTest));
        dataPkt.packPacketToEncodedBytes();
        comm.sendDataPacket(&dataPkt);
        qDebug()<<"StructSz:"<<sizeof(CTTest)<<dataPkt.msgData<<"|";
        }
        break;
    default:
        break;
    }
}
