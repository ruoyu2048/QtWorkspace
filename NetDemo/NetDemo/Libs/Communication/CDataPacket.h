#ifndef CPACKET_H
#define CPACKET_H
#include <QList>
#include <QByteArray>

class CDataPacket
{
public:
    CDataPacket();
    /**
     * @brief bytesToPacket--将字加后>的节序转换成数据报文对象
     * @param rcvAry--接受到的字节序
     */
    void bytesToPacket(QByteArray rcvAry);

    /**
     * @brief packetToBytes--将报文对象转换成字节序
     * @return
     */
    QByteArray packetToBytes();

    /**
     * @brief encodeBytesToPacket--执行转码后的完整报文字节序转成CDataPacket对象
     * @param encodeAry--转码后的完整报文字节序
     */
    void encodeBytesToPacket(QByteArray encodeAry);

    /**
     * @brief encodePacketToBytes--执行转码后的CDataPacket对象转成完整报文字节序
     * @return --转码后的完整报文字节序
     */
    QByteArray encodePacketToBytes();

    void decodeBytesToPacket(QByteArray realAry);

    QByteArray decodePacketToBytes();

    /**
     * @brief encodeData--对数据进行转码
     */
    void encodeData();

    /**
     * @brief decodeData--对数据进行解码
     * @return --解码成功返回true,否则返回false
     */
    bool decodeData();

    /**
     * @brief makeRegisterPacket--制作注册报文对象
     * @param msgTypeReg--订阅的消息类型
     * @param dstIdList--目的地址列表
     * @return --注册报文字节序
     */
    QByteArray makeRegisterPacket( QList<quint8> dstIdList );

    bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
    bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);

private:
    /**
     * @brief checkBitTest--校验位检验
     * @return --校验位正确返回true,否则返回false
     */
    bool checkBitTest();
    /**
     * @brief getLowByte--获取低字节
     * @param val--无符号短整型数值
     * @return --低字节数值
     */
    quint8 getLowByte(quint16 val);

    /**
     * @brief getHighByte--获取高字节
     * @param msgLen--无符号短整型数值
     * @return -高字节数值
     */
    quint8 getHighByte(quint16 val);

public:
    quint8 msgHead;
    quint8 msgDst;
    quint8 msgSrc;
    quint8 msgType;
    quint16 msgLen;
    QByteArray  msgData;
    quint8 msgCheck;
    quint8 msgEnd;
};

#endif // CPACKET_H
