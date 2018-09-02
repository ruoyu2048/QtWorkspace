#ifndef CPACKET_H
#define CPACKET_H
#include <QList>
#include <QByteArray>

//#define COMMAXLEN 1024

class CDataPacket
{
public:
    //CDataPacket();
    void bytesToPacket(QByteArray rcvAry);
    QByteArray packetToBytes();
    QByteArray makeRegisterPacket( quint8 msgTypeReg,QList<quint8> dstIdList );

    bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
    bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
public:
    quint8 msgHead;
    quint8 msgDst;
    quint8 msgSrc;
    quint8 msgType;
    quint16 msgLen;
    QByteArray  msgData;
    quint8 msgCheck;
    quint8 msgEnd;
private:
    unsigned char msgAry[1024];
};

#endif // CPACKET_H
