#ifndef CPACKET_H
#define CPACKET_H
#include <QList>
#include <QByteArray>

class CDataPacket
{
public:
    CDataPacket();

    //转码后的字节流---->转码后的对象
    /**
     * @brief setEncodedPacketBytes--设置转码后的完整数据包字节流
     * @param encodedBytes--码后的完整数据包字节流
     */
    void setEncodedPacketBytes(QByteArray encodedBytes);

    /**
     * @brief checkBitTest--校验位验证
     * @return --正确返回true,错误返回false
     */
    bool checkBitTest();

    /**
     * @brief encodedBytesToPacket--将转码后的字节流转换成报文对象
     */
    void encoddeBytesToPacket();

    /**
     * @brief decodeData--对数据进行解码
     * @return --解码成功返回true,否则返回false
     */
    bool decodeData();


    //转码前的对象---->转码后的字节流
    /**
     * @brief packPacketToEncodedBytes--未转码的包对象转乘字节流
     * @return
     */
    QByteArray packPacketToEncodedBytes();

    /**
     * @brief makeRegisterPacket--制作注册报文对象
     * @param msgTypeReg--订阅的消息类型
     * @param dstIdList--目的地址列表
     * @return --注册报文字节序
     */
    QByteArray makeRegisterPacket( QList<quint8> dstIdList );

private:
    /**
     * @brief encodeData--对数据进行转码
     */
    void encodeData();

    QByteArray encodePacketToBytes();

    bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
    bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);

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
    quint8      msgHead;
    quint8      msgDst;
    quint8      msgSrc;
    quint8      msgType;
    /**
     * @brief msgLen--调用packPacketToEncodedBytes方法后，msgLen=长度位与信息区转码后的总长度
     *                调用decodeData方法后，msgLen=解码后的信息区实际长度
     */
    quint16     msgLen;
    /**
     * @brief msgData--调用packPacketToEncodedBytes方法后，msgData=长度位与信息区转码后的字节流
     *                调用decodeData方法后，msgData=解码后的信息区实际字节流
     */
    QByteArray  msgData;
    quint8      msgCheck;
    quint8      msgEnd;

public:
    QByteArray encodedPacketBytes;

private:
    QByteArray encodedLengthDataBytes;
};

#endif // CPACKET_H
