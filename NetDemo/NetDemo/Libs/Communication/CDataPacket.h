#ifndef CPACKET_H
#define CPACKET_H
#include <QList>
#include <QByteArray>

class CDataPacket
{
public:
    //CDataPacket();
    /**
     * @brief bytesToPacket--将字节序转换成数据报文对象
     * @param rcvAry--接受到的字节序
     */
    void bytesToPacket(QByteArray rcvAry);

    /**
     * @brief packetToBytes--将报文对象转换成字节序
     * @return
     */
    QByteArray packetToBytes();

    /**
     * @brief makeRegisterPacket--制作注册报文对象
     * @param msgTypeReg--订阅的消息类型
     * @param dstIdList--目的地址列表
     * @return --注册报文字节序
     */
    QByteArray makeRegisterPacket( QList<quint8> dstIdList );

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
