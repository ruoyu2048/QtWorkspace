#ifndef CDATATRANSCODE_H
#define CDATATRANSCODE_H
#include <QList>
#include <QByteArray>
#include <cstring>

class CDataTranscode
{
public:
    CDataTranscode();

    /**
     * @brief encodeData 转码
     * @param decodeDataAry 转码前的报文，格式：报头+目的地址+原地址+消息类型+信息区长度+信息区+报尾
     * @return
     */
    bool encodeData( QByteArray decodeDataAry );

    /**
     * @brief decodeData 解码
     * @param encodeDataAry 转码后的报文，格式：报头+目的地址+原地址+消息类型+信息区长度+信息区+校验位+报尾
     * @return
     */
    bool decodeData( QByteArray encodeDataAry );//对数据进行解码--解码成功返回true,否则返回false

    /**
     * @brief parseDataToStruct 将解码后的信息区报文转换成结构体
     * @param infoDataStruct
     * @param nStructSz
     */
    void parseDataToStruct(uchar* infoDataStruct,int nStructSz);//将转码后的信息区转换成信息区结构体

    /**
     * @brief getEncodeDataAry 获取转码后的数据,格式：报头+目的地址+原地址+消息类型+信息区长度+信息区+校验位+报尾
     * @return
     */
    QByteArray getEncodeDataAry();//获取转码后的数据，包含报头、报尾等

    /**
     * @brief getDecodeDataAry //获取解码后的数据格式：报头+目的地址+原地址+消息类型+信息区长度+信息区+报尾
     * @return
     */
    QByteArray getDecodeDataAry();

    /**
     * @brief isToLocal 是否为回执报文
     * @param msgDst 接收报文的目的地址
     * @return
     */
    bool isToLocal( quint8 msgDst );

private:
    /**
     * @brief checkBitTest 校验位检测
     * @param encodeDataAry 转码后的报文
     * @return
     */
    bool checkBitTest( QByteArray encodeDataAry );
    bool SendBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
    bool RecvBufChange(unsigned char* prefBuf,unsigned char* AfterBuf,short int PreLen,short int& AfterLen);
    quint8 getLowByte(quint16 val);//获取低字节
    quint8 getHighByte(quint16 val);//获取高字节

public:
    quint8      m_msgHead;
    quint8      m_msgDst;
    quint8      m_msgSrc;
    quint8      m_msgType;
    quint16     m_msgLen;
    QByteArray  m_msgData;
    quint8      m_msgCheck;
    quint8      m_msgEnd;

private:
    QByteArray  m_encodeAry;//加密后的数据
    QByteArray  m_decodeAry;//解密后的数据
};

#endif // CDATATRANSCODE_H
