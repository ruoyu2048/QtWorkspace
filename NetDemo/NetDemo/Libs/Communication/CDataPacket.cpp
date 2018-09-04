#include "CDataPacket.h"
#include <cstring>
#include "DataStruct.h"
//#include <windows.h>

CDataPacket::CDataPacket(){

}

void CDataPacket::bytesToPacket(QByteArray rcvAry){
    msgHead = rcvAry[0];
    msgDst  = rcvAry[1];
    msgSrc  = rcvAry[2];
    msgType = rcvAry[3];
    /*信息区长度字节序：前低后高*/
    msgLen = (uchar)rcvAry[4]+((uchar)rcvAry[5]<<8);
    msgData = rcvAry.mid(6,msgLen);
    msgCheck = rcvAry[rcvAry.length()-2];
    msgEnd = rcvAry[rcvAry.length()-1];
}

QByteArray CDataPacket::packetToBytes(){
    QByteArray dataAry;
    dataAry.append(msgHead);
    dataAry.append(msgDst);
    dataAry.append(msgSrc);
    dataAry.append(msgType);
    //信息区长度字节序：低位在前，高位在后
    dataAry.append(getLowByte(msgLen));
    dataAry.append(getHighByte(msgLen));
    dataAry.append(msgData);
    dataAry.append(msgCheck);
    dataAry.append(msgEnd);

    return dataAry;
}

void CDataPacket::encodeData(){
    //完整报文
    QByteArray msgAry = packetToBytes();
}

bool CDataPacket::decodeData(){
    //校验位检验
    if( !checkBitTest() )
        return false;
    //对信息区进行解码

}

QByteArray CDataPacket::makeRegisterPacket( QList<quint8> dstIdList ){
    msgHead = 0xAA;
    msgDst  = 0xAB;
    msgSrc  = 0xAB;
    msgType = 0xFF;
    msgLen  = dstIdList.length();

    foreach (quint8 dstId, dstIdList) {
        msgData.append(dstId);
    }

    msgCheck = 0xA4;
    msgEnd   = 0xA5;

    return packetToBytes();
}

//添加附加字
bool CDataPacket::SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen)
{
    //cout<<"nPreLen:"<<PreLen<<" PreBuf:"<<prefBuf<<endl;
    unsigned char DataBeforeTrans[COMMAXLEN];
    unsigned char DataAfterTrans[COMMAXLEN];

    memset(DataBeforeTrans,0,COMMAXLEN);
    memset(DataAfterTrans,0,COMMAXLEN);

    // 计算转换后发送数据的长度
    quint16 DataLen= PreLen + 2;//信息区长度（PreLen）+长度（2字节）=sizeof(short)
    AfterLen =DataLen + ((DataLen) / 7);
    if ((DataLen % 7) > 0){
        AfterLen = AfterLen + 1;
    }
//    DataBeforeTrans[0]=HIBYTE((WORD)(AfterLen-2));
//    DataBeforeTrans[1]=LOBYTE((WORD)(AfterLen-2));
    int nDataLen = AfterLen - 2;//转码后信息区的实际长度 = AfterLen - 长度（2字节=sizeof(short)）
//    DataBeforeTrans[0]=(uchar)((nDataLen&0xff00)>>8);
//    DataBeforeTrans[1]=(uchar)(nDataLen&0x00ff);
    //低字节在前，高字节在后
    DataBeforeTrans[0] = nDataLen&0x00ff;
    DataBeforeTrans[1] = (nDataLen&0xff00)>>8;
    memmove(DataBeforeTrans+2,prefBuf,PreLen);

    // 数据长度-信息区：数据转换，得到附加字
    int j = 0;
    for (int i = 0; i < DataLen; i++){
        DataAfterTrans[j++] = DataBeforeTrans[i];
        // 每7个字节，进行数据转换，得到附加字
        if ((i + 1) % 7 == 0){
            int k = j - 7;
            int n = 1;
            for (int m = 0; m < 7; m++){
                //判断操作系统的字节序
                if((DataAfterTrans[k] | 0x80) == DataAfterTrans[k]){
                    DataAfterTrans[k] &= 0x7f;
                    DataAfterTrans[j] |= n % 256;
                }
                k++;
                n = n * 2;
            }
            j++;
        }
        //不足7字节处理
        else if (j== AfterLen - 1){
            int s = DataLen % 7;
            int k = j - s;
            int n = 1;
            for (int m = 0; m < s; m++){
                if((DataAfterTrans[k] | 0x80) == DataAfterTrans[k]){
                    DataAfterTrans[k] &= 0x7f;
                    DataAfterTrans[j] |= n % 256;
                }
                k++;
                n = n * 2;
            }
            j++;
        }
        //cout<<"B__AfterLen:"<<AfterLen<<" DAT:"<<DataAfterTrans+((i/7)*8)<<"|"<<i<<" "<<j<<endl<<endl;
    }
    memmove(AfterBuf,DataAfterTrans,AfterLen);

    return true;
}


// 附加字接解码，提取长度区和信息区实际数据，AfterLen=长度区和信息区的总长度
bool CDataPacket::RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen)
{
    // 提取长度区和信息区实际数据
    unsigned char RecvBuf[COMMAXLEN];
    short BufLen = 0;

    unsigned char BeforeBuf[COMMAXLEN];
    //计算接收到报文的长度与信息区的总长度 = (接收到的报文总长度-报头(帧头+目的+源+类型)-校验位-帧尾)
    int iBeforeLen = PreLen-6;//==PreLen-4-1-1;
    memmove(BeforeBuf,prefBuf+4,iBeforeLen);//长度和信息区
    short sTemp = iBeforeLen/8;//需要几次循环
    for (int j=0;j<sTemp;j++){
        unsigned char cTemp[8];
        memmove(cTemp,BeforeBuf+8*j,8);
        for (int k=0;k<7;k++){
            if ((cTemp[7]&0x01) != 0){
                RecvBuf[BufLen] = cTemp[k] + 0x80;
            }
            else{
                RecvBuf[BufLen] = cTemp[k];
            }
            cTemp[7] = cTemp[7]>>1;
            BufLen++;
        }
    }
    sTemp = iBeforeLen%8;//如果有余数
    if (sTemp != 0){
        unsigned char cTemp[8];
        memmove(cTemp,BeforeBuf+(iBeforeLen - sTemp),sTemp);
        for (int k=0;k<sTemp-1;k++){
            if ((cTemp[sTemp-1]&0x01) != 0){
                RecvBuf[BufLen] = cTemp[k] + 0x80;
            }
            else{
                RecvBuf[BufLen] = cTemp[k];
            }
            cTemp[sTemp-1] = cTemp[sTemp-1]>>1;
            BufLen++;
        }
    }
    memmove(AfterBuf,RecvBuf,BufLen);
    AfterLen=BufLen;

    return true;
}

bool CDataPacket::checkBitTest(){

}

quint8 CDataPacket::getLowByte(quint16 val){
    return val&0xFF;
}

quint8 CDataPacket::getHighByte(quint16 val){
    return (val>>8)&0xFF;
}
