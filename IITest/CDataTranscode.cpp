#include "CDataTranscode.h"
#include <QDebug>
#include "globaldef.h"

CDataTranscode::CDataTranscode()
{
}

bool CDataTranscode::encodeData(QByteArray decodeDataAry)
{
    unsigned char DataBeforeTrans[COMMAXLEN] = {'\0'};
    unsigned char DataAfterTrans[COMMAXLEN] = {'\0'};

    QByteArray msgData=decodeDataAry.mid(6,decodeDataAry.length()-6-1);//加密前的报文(包含报头、目的地址、原地址、消息类型、信息去长度、信息区、报尾）

    //转码前的实际数据长度
    quint16 nMsgBeforeLen = msgData.length();
    //长度位 + 转码<前>信息区的总长度
    quint16 nTotalBeforeLen = sizeof(quint16) + nMsgBeforeLen;
    //长度位 + 转码<后>信息区的总长度
    quint16 nTotalAfterLen = nTotalBeforeLen + ((nTotalBeforeLen)/7);
    if( nTotalBeforeLen%7 > 0 ){
        nTotalAfterLen = nTotalAfterLen + 1;
    }

    //转码<后>信息区长度
    DataBeforeTrans[0] = getHighByte(nTotalAfterLen-2);
    DataBeforeTrans[1] = getLowByte(nTotalAfterLen-2);
    memmove(DataBeforeTrans+2,(unsigned char*)msgData.data(),nMsgBeforeLen);

    // 数据长度-信息区：数据转换，得到附加字
    int j = 0;
    for (int i = 0; i < nTotalBeforeLen; i++){
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
        else if (j== nTotalAfterLen - 1){
            int s = nTotalBeforeLen % 7;
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
    }

    m_msgData.clear();
    m_msgHead  = decodeDataAry[0];
    m_msgDst   = decodeDataAry[1];
    m_msgSrc   = decodeDataAry[2];
    m_msgType  = decodeDataAry[3];
    m_msgLen   = nTotalAfterLen;//(长度位+信息区)转码后的长度
    m_msgData.append((char*)DataAfterTrans,nTotalAfterLen);

    //校验位计算
    QByteArray checkAry=decodeDataAry.mid(1,3);
    checkAry.append(m_msgData);//转码后的msgData包含msgLen
    quint16 nTotalLen = checkAry.length();
    quint8  nCheck    = checkAry.at(0);
    for( int i=1; i<nTotalLen; i++ ){
        nCheck ^= (quint8)checkAry.at(i);
    }
    m_msgCheck = nCheck;
    m_msgEnd   = decodeDataAry[decodeDataAry.size()-1];

    m_encodeAry.clear();
    m_encodeAry.append(m_msgHead);
    m_encodeAry.append(m_msgDst);
    m_encodeAry.append(m_msgSrc);
    m_encodeAry.append(m_msgType);
    m_encodeAry.append(m_msgData);
    m_encodeAry.append(m_msgCheck);
    m_encodeAry.append(m_msgEnd);

    return true;
}

bool CDataTranscode::decodeData(QByteArray encodeDataAry)
{
    // 提取长度区和信息区实际数据
    unsigned char RecvBuf[COMMAXLEN] = {'\0'};;
    short BufLen = 0;

    unsigned char BeforeBuf[COMMAXLEN] = {'\0'};;
    QByteArray encodedLengthDataBytes = encodeDataAry.mid(4,encodeDataAry.length()-4-1-1);
    //对信息区进行解码
    quint16 iBeforeLen = encodedLengthDataBytes.length();
    memmove(BeforeBuf,(unsigned char*)encodedLengthDataBytes.data(),iBeforeLen);//长度和信息区
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

    if( checkBitTest(encodeDataAry) ){
        m_msgData.clear();
        m_msgHead  = encodeDataAry[0];
        m_msgDst   = encodeDataAry[1];
        m_msgSrc   = encodeDataAry[2];
        m_msgType  = encodeDataAry[3];

        m_msgLen = BufLen-sizeof(quint16);
        m_msgData.append((char*)RecvBuf+2,m_msgLen);

        m_msgCheck = encodeDataAry[encodeDataAry.length()-2];
        m_msgEnd   = encodeDataAry[encodeDataAry.length()-1];

        m_decodeAry.clear();
        m_decodeAry.append(m_msgHead);
        m_decodeAry.append(m_msgDst);
        m_decodeAry.append(m_msgSrc);
        m_decodeAry.append(m_msgType);
        m_decodeAry.append((char*)&m_msgLen,sizeof(short));
        m_decodeAry.append(m_msgData);
        //m_decodeAry.append(m_msgCheck);
        m_decodeAry.append(m_msgEnd);
        return true;
    }

    return false;
}


void CDataTranscode::parseDataToStruct(uchar* infoDataStruct, int nStructSz)
{
    if(m_msgData.length()>=nStructSz)
    {
        memcpy(infoDataStruct,(uchar*)m_msgData.data(),nStructSz);
        m_msgLen = nStructSz;
    }
    else
    {
        memcpy(infoDataStruct,(uchar*)m_msgData.data(),m_msgData.length());
        m_msgLen = m_msgData.length();
    }
}

QByteArray CDataTranscode::getEncodeDataAry()
{
    return m_encodeAry;
}

QByteArray CDataTranscode::getDecodeDataAry()
{
    return m_decodeAry;
}

bool CDataTranscode::isToLocal(quint8 msgDst)
{
    return (m_msgSrc==msgDst);
}

void CDataTranscode::clearEncodeAry()
{
    m_encodeAry.clear();
}

void CDataTranscode::clearDecodeAry()
{
    m_decodeAry.clear();
}

bool CDataTranscode::checkBitTest( QByteArray encodeDataAry ){
    quint16 nTotalLen  = encodeDataAry.length();
    if( nTotalLen >= 3 ){
        quint8  nCheck = encodeDataAry.at(1);
        for( int i=2;i<nTotalLen-2;i++ ){
            nCheck ^= (quint8)encodeDataAry.at(i);
        }

        if( nCheck == (quint8)encodeDataAry.at(nTotalLen-2) )
            return true;
        qDebug()<<"校验位错误:[计算值："<<(char)nCheck<<"报文值："<<(char)encodeDataAry.at(nTotalLen-2)<<"]";
    }
   return false;
}
//添加附加字
bool CDataTranscode::SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen)
{
    unsigned char DataBeforeTrans[COMMAXLEN];
    unsigned char DataAfterTrans[COMMAXLEN];

    memset(DataBeforeTrans,0,COMMAXLEN);
    memset(DataAfterTrans,0,COMMAXLEN);

    // 计算转换后发送数据的长度
    quint16 DataLen= PreLen + 2;//信息区长度（PreLen）+长度（2字节）=sizeof(short)
    AfterLen =DataLen + ((DataLen) / 7);
    if ((DataLen % 7) > 0)
    {
        AfterLen = AfterLen + 1;
    }

    int nDataLen = AfterLen - 2;//转码后信息区的实际长度 = AfterLen - 长度（2字节=sizeof(short)）
    //低字节在前，高字节在后
    DataBeforeTrans[0] = getHighByte(nDataLen);
    DataBeforeTrans[1] = getLowByte(nDataLen);

    memmove(DataBeforeTrans+2,prefBuf,PreLen);

    // 数据长度-信息区：数据转换，得到附加字
    int j = 0;
    for (int i = 0; i < DataLen; i++)
    {
        DataAfterTrans[j++] = DataBeforeTrans[i];
        // 每7个字节，进行数据转换，得到附加字
        if ((i + 1) % 7 == 0)
        {
            int k = j - 7;
            int n = 1;
            for (int m = 0; m < 7; m++)
            {
                //判断操作系统的字节序
                if((DataAfterTrans[k] | 0x80) == DataAfterTrans[k])
                {
                    DataAfterTrans[k] &= 0x7f;
                    DataAfterTrans[j] |= n % 256;
                }
                k++;
                n = n * 2;
            }
            j++;
        }
        //不足7字节处理
        else if (j== AfterLen - 1)
        {
            int s = DataLen % 7;
            int k = j - s;
            int n = 1;
            for (int m = 0; m < s; m++)
            {
                if((DataAfterTrans[k] | 0x80) == DataAfterTrans[k])
                {
                    DataAfterTrans[k] &= 0x7f;
                    DataAfterTrans[j] |= n % 256;
                }
                k++;
                n = n * 2;
            }
            j++;
        }
    }
    memmove(AfterBuf,DataAfterTrans,AfterLen);//AfterLen=长度区和信息区的总长度,AfterBuf包括长度+信息区

    return true;
}
// 附加字接解码，提取长度区和信息区实际数据，AfterLen=长度区和信息区的总长度,AfterBuf包括长度+信息区
bool CDataTranscode::RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen)
{
    // 提取长度区和信息区实际数据
    unsigned char RecvBuf[COMMAXLEN];
    short BufLen = 0;

    unsigned char BeforeBuf[COMMAXLEN];
    int iBeforeLen=0;
    //计算接收到报文的长度与信息区的总长度 = (接收到的报文总长度-报头(帧头+目的+源+类型)-校验位-帧尾)
    memmove(BeforeBuf,prefBuf+4,PreLen-6);//长度和信息区int
    iBeforeLen = PreLen-6;
    short sTemp = iBeforeLen/8;//需要几次循环
    for (int j=0;j<sTemp;j++)
    {
        unsigned char cTemp[8];
        memmove(cTemp,BeforeBuf+8*j,8);
        for (int k=0;k<7;k++)
        {
            if ((cTemp[7]&0x01) != 0)
            {
                RecvBuf[BufLen] = cTemp[k] + 0x80;
            }
            else
            {
                RecvBuf[BufLen] = cTemp[k];
            }
            cTemp[7] = cTemp[7]>>1;
            BufLen++;
        }
    }
    sTemp = iBeforeLen%8;//如果有余数
    if (sTemp != 0)
    {
        unsigned char cTemp[8];
        memmove(cTemp,BeforeBuf+(iBeforeLen - sTemp),sTemp);
        for (int k=0;k<sTemp-1;k++)
        {
            if ((cTemp[sTemp-1]&0x01) != 0)
            {
                RecvBuf[BufLen] = cTemp[k] + 0x80;
            }
            else
            {
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

quint8 CDataTranscode::getLowByte(quint16 val)
{
    return val&0xFF;
}

quint8 CDataTranscode::getHighByte(quint16 val)
{
    quint8 value=(val>>8)&0xFF;
    return value;
}
