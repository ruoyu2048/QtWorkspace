#include "CTcpServer.h"
#include "../PubDef/PubFunc.h"
//#include "PubFunc.h"

CTcpServer::CTcpServer(QObject *parent) : QTcpServer(parent)
{

}

/************************************************************************
*函数名:	StartListen
*概述:启动TCP Server侦听
*参数：strHostIP--TCP服务端IP地址
*     nHostPort--TCP服务端端口
*返回值：如果启动成功，则返回true，否则返回false
************************************************************************/
bool CTcpServer::StartListen(QString strServerIP,quint16 nServerPort)
{
    QHostAddress hostAddr(strServerIP);
    if(true == this->listen(hostAddr,nServerPort))
    //if(true == this->listen(QHostAddress::Any,nServerPort))
    //if(true == this->listen(QHostAddress("127.0.0.1"),nServerPort))
    {
        qDebug()<<"Server started";
        return true;
    }

    return false;
}

/************************************************************************
*函数名:	SendData
*概述:向客户端发送数据报文
*参数：handle--目标套接字标识符
*     sendBuf--发送报文
*     nSendLen--发送报文长度
*返回值：如果启动成功，则返回true，否则返回false
************************************************************************/
bool CTcpServer::SendData(qintptr handle,unsigned char* sendBuf,int nSendLen)
{
    QMap<qintptr,CTcpSocket*>::iterator it = mClientsMap.find(handle);
    if( it != mClientsMap.end() )
    {
        int nRet = it.value()->SendData(sendBuf,nSendLen);
        if( -1 != nRet )
            return true;
    }
    return false;
}


void CTcpServer::StopListen()
{
    this->close();
}

/************************************************************************
*函数名:	incomingConnection
*概述:QTcpServer中虚函数
*参数：handle--TCP套接字描述符
*返回值：无
************************************************************************/
void CTcpServer::incomingConnection(qintptr handle)
{
    CTcpSocket* pNewSocket = new CTcpSocket(this);
    if( NULL != pNewSocket )
    {
        if( true == pNewSocket->setSocketDescriptor(handle) )
        {
            pNewSocket->SaveSocketDecriptor();
            pNewSocket->DisplaySocketDecriptor();
            connect(pNewSocket,SIGNAL(signalSendData(qintptr,unsigned char*,int)),this,SLOT(slotReadData(qintptr,unsigned char*,int)));
            connect(pNewSocket,SIGNAL(signalDisconnected(qintptr)),this,SLOT(slotDisconnected(qintptr)));
            mClientsMap.insert(handle,pNewSocket);
            qDebug()<<pNewSocket->peerAddress().toString()<<" PORT="<<pNewSocket->peerPort();
        }
    }
}

/************************************************************************
*函数名:	slotReadData
*概述:接收自定义套接字中的发送的报文
*参数：handle--TCP套接字描述符
*返回值：无
************************************************************************/
void CTcpServer::slotReadData(qintptr handle,unsigned char* rcvBuf,int nRcvLen)
{
    QMap<qintptr,CTcpSocket*>::iterator it = mClientsMap.find(handle);
    if( it != mClientsMap.end() )
    {
        //eg:
        unsigned char cBuf[COMMAXLEN] = {'0'};
        memmove(cBuf,rcvBuf,nRcvLen);
        //qDebug()<<"SERVER:"<<cBuf[0]<<cBuf[1]<<cBuf[2]<<cBuf[3]<<nRcvLen;
    }
}

/************************************************************************
*函数名:	slotDisconnected
*概述:清除队列中断开连接的客户端套接字
*参数：handle--TCP套接字描述符
*返回值：无
************************************************************************/
void CTcpServer::slotDisconnected(qintptr handle)
{
    QMap<qintptr,CTcpSocket*>::iterator it = mClientsMap.find(handle);
    if( it != mClientsMap.end() )
    {
        mClientsMap.remove(it.key());
        qDebug()<<"CTcpServer::Left="<<mClientsMap.size();
    }
}

CTcpSocket::CTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    mRcvLen = 0;
    connect(this,SIGNAL(readyRead()),this,SLOT(slotReadData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

/************************************************************************
*函数名:	SaveSocketDecriptor
*概述:将该套接字的描述符保存到成员变量，在TCPSever中要用到。
*参数：无
*返回值：无
************************************************************************/
void CTcpSocket::SaveSocketDecriptor()
{
    mSocketDecriptor = this->socketDescriptor();
}

/************************************************************************
*函数名:	DisplaySocketDecriptor
*概述:打印套接字描述符(测试)。
*参数：无
*返回值：无
************************************************************************/
void CTcpSocket::DisplaySocketDecriptor()
{
    qDebug()<<"socketDescriptor:"<<this->socketDescriptor();
}

/************************************************************************
*函数名:	slotReadData
*概述:SendData
*参数：无
*返回值：无
************************************************************************/
bool CTcpSocket::SendData(unsigned char* sendBuf,int nSendLen)
{
    qint64 nRet = this->write((char*)sendBuf,nSendLen);
    if( -1 == nRet )
        return false;

    return true;
}

//slots
/************************************************************************
*函数名:	slotReadData
*概述:接收数据报文。
*参数：无
*返回值：无
************************************************************************/
void CTcpSocket::slotReadData()
{
    if( this->bytesAvailable() > sizeof(FrameHead) )
    {
        QByteArray rcvAry = this->readAll();
        mRcvLen += rcvAry.length();
        qDebug()<<"RecvLength:"<<mRcvLen;
        //parseDatagram(rcvAry);
    }
}

void CTcpSocket::slotDisconnected()
{
    qDebug()<<"CTcpSocket::slotDisconnected()";
    //emit signalDisconnected(this->socketDescriptor());
    //发送断开连接信号
    emit signalDisconnected(mSocketDecriptor);
}

void CTcpSocket::parseDatagram(QByteArray rcvAry)
{
    //将获取到的报文添加到缓存中
    mCacheAry.append(rcvAry);
    //计算缓存长度
    int nCacheLen = mCacheAry.length();
    while( nCacheLen > 0 )
    {
        //解析缓存
        int nMaxRcvBufLen = nCacheLen;
        unsigned char cRcvBuf[COMMAXLEN] = {'0'};
        if( nCacheLen >= COMMAXLEN ){
            nMaxRcvBufLen = COMMAXLEN;
        }
        memmove(cRcvBuf,(unsigned char*)mCacheAry.data(),nMaxRcvBufLen);
        /*---------------------------------报文数据完整性验证---------------------------------*/
        //先找帧头
        bool bFindHead = true;
        if( 0xAA != cRcvBuf[0] ){//帧头不正确
            qDebug()<<"帧头不正确 [Head:"<<cRcvBuf[0]<<"]";
            bFindHead = false;
        }
        //再找帧尾
        int nTotalLen = 0;
        bool bFindTail = false;
        for( ;nTotalLen<nMaxRcvBufLen;nTotalLen++ ){
            if( 0xA5 == cRcvBuf[nTotalLen] ){
                ++nTotalLen;
                bFindTail = true;
                break;
            }
        }

        //有效报文(有头有尾)
        if( true == bFindHead && true ==bFindTail )
        {
            //计算校验位
//            unsigned char cCheck = cRcvBuf[1];
//            for(int i=2;i<nTotalLen-2;i++){
//                cCheck = cCheck^cRcvBuf[i];
//            }

            int nStartPos = 4;
            unsigned char cCheck = cRcvBuf[nStartPos];
            for(int i=nStartPos+1;i<nTotalLen-2;i++){
                cCheck += cRcvBuf[i]&0xFF;
            }
            cCheck = cCheck&0xFF;

            //比较校验位
            if( cCheck != cRcvBuf[nTotalLen-2] ){//校验位判断
                qDebug()<<"校验位错误 [cCheck:"<<cCheck<<"SrcCheck:"<<cRcvBuf[nTotalLen-2]<<"]";
                qDebug()<<"mCacheAry:"<<mCacheAry<<"\n";
                mCacheAry.remove(0,nTotalLen);//将错误信息丢弃
                nCacheLen = mCacheAry.length();
                continue;
            }
            /*---------------------------------解析报文---------------------------------*/
            switchDatagram(cRcvBuf, nTotalLen);
            //将解析过的报文数据从缓存中清除
            mCacheAry.remove(0,nTotalLen);
            //重新计算缓存中的报文数据长度
            nCacheLen = mCacheAry.length();
        }
        else
        {
            //有头无尾，则跳过
            if( true == bFindHead && false == bFindTail ){
                break;
            }
            else{
                //无头无尾、无头有尾，则丢弃
                QByteArray invalidData = mCacheAry.mid(0,nTotalLen);
                qDebug()<<"InvalidDataLen:"<<nTotalLen<<"InvalidData:"<<invalidData;
                //将无效的报文数据从缓存中丢弃
                mCacheAry.remove(0,nTotalLen);
                //重新计算缓存中的报文数据长度
                nCacheLen = mCacheAry.length();
                qDebug()<<"CurCacheLen:"<<nCacheLen<<"CurCacheData:"<<mCacheAry<<"\n";
                continue;
            }
        }
    }
}

void CTcpSocket::switchDatagram(unsigned char* cRcvBuf,int nTotalLen)
{
    //信息区
    short nInfoLen = 0;//信息长度=长度位(2)+信息区位长度(N)
    unsigned char cInfoBuf[COMMAXLEN] = {'0'};//解码后报文=转码后的长度+信息区
    RecvBufChange(cRcvBuf,cInfoBuf,nTotalLen,nInfoLen);

    //帧头
    FrameHead head;
    memmove((unsigned char*)&head,cRcvBuf,sizeof(FrameHead));
    //报头信息(帧头、目的地址、源地址、类型)
    //qDebug()<<head.cHead<<head.cDesAdd<<head.cSrcAdd<<head.cType;

    switch (head.cType) {
    case 0x32:{
        MidInfo midInfo;
        memmove((unsigned char*)&midInfo,cInfoBuf+sizeof(short),sizeof(MidInfo));
        qDebug()<<midInfo.cLDState<<midInfo.cWorkMode<<midInfo.cPulseInput<<midInfo.cOLMid<<midInfo.cDSMid;
    }
        break;
    case 0x33:
        break;
    }

    //eg:
    emit signalSendData(mSocketDecriptor,(unsigned char*)&head,sizeof(FrameHead));
}
