#ifndef CPACKET_H
#define CPACKET_H
#include <QByteArray>

//#define COMMAXLEN 1024

class CDataPacket
{
public:
    CDataPacket();
    bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
    bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
public:
    quint8      msgType;
    QByteArray  msgData;
};

#endif // CPACKET_H
