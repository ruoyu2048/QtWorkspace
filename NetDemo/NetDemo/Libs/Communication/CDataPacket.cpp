#include "CDataPacket.h"
#include <cstring>
#include "PubFunc.h"
//#include <windows.h>

//CDataPacket::CDataPacket(){

//}

void CDataPacket::bytesToPacket(QByteArray rcvAry){
    msgHead = rcvAry[0];
    msgDst  = rcvAry[1];
    msgSrc  = rcvAry[2];
    msgType = rcvAry[3];
    msgLen = ((uchar)rcvAry[4]<<8) + (uchar)rcvAry[5];
    msgData = rcvAry.mid(6,msgLen);
    msgCheck = rcvAry[rcvAry.length()-2];
    msgEnd = rcvAry[rcvAry.length()-1];
}

QByteArray CDataPacket::packetToBytes(){
    memset(&msgAry,'\0',sizeof(msgAry));
    msgAry[0] = msgHead;
    msgAry[1] = msgDst;
    msgAry[2] = msgSrc;
    msgAry[3] = msgType;
    msgAry[4] = msgLen>>8;
    msgAry[5] = msgLen&0x00ff;
    memmove(msgAry+6,msgData.data(),msgLen);
    msgAry[6+msgLen] = msgCheck;
    msgAry[6+msgLen+1] = msgEnd;

    QByteArray dataAry;
    dataAry.append((char*)msgAry,4+2+msgLen+2);
    return dataAry;
}

QByteArray CDataPacket::makeRegisterPacket( quint8 msgTypeReg,QList<quint8> dstIdList ){
    msgHead = 0xAA;
    msgDst = 0xAB;
    msgSrc = 0xAB;
    msgType = msgTypeReg;

    msgLen = dstIdList.length();
    foreach (quint8 dstId, dstIdList) {
        char cDstId = dstId;
        msgData.append(cDstId);
    }
    msgCheck = 0xA4;
    msgEnd = 0xA5;

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
    short int DataLen= PreLen + 2;//信息区长度（PreLen）+长度（2字节）=sizeof(short)
    AfterLen =DataLen + ((DataLen) / 7);
    if ((DataLen % 7) > 0){
        AfterLen = AfterLen + 1;
    }
//    cout<<"AfterLen:"<<AfterLen<<" PreBuf:"<<prefBuf<<endl;
//    DataBeforeTrans[0]=HIBYTE((WORD)(AfterLen-2));
//    DataBeforeTrans[1]=LOBYTE((WORD)(AfterLen-2));
    int nDataLen = AfterLen - 2;//转码后信息区的实际长度 = AfterLen - 长度（2字节=sizeof(short)）
    DataBeforeTrans[0]=(char)((nDataLen&0xff00)>>8);
    DataBeforeTrans[1]=(char)(nDataLen&0x00ff);
    memmove(DataBeforeTrans+2,prefBuf,PreLen);
    //cout<<"AfterLen:"<<AfterLen<<" DBT:"<<DataBeforeTrans<<endl;

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
