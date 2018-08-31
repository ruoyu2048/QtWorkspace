#include "CTcpServer.h"
#include "CTcpThread.h"
#include "DataStruct.h"
#include "PubFunc.h"

CTcpServer::CTcpServer(QObject *parent) : QTcpServer(parent){
    qRegisterMetaType<qintptr>("qintptr");
}

bool CTcpServer::startListen(QString strServerIP,quint16 nServerPort){
    QHostAddress hostAddr(strServerIP);
    if(true == this->listen(hostAddr,nServerPort)){
        qDebug()<<"Server Started Sucessfully !";
        return true;
    }
    qDebug()<<"Tcp Server Started Failed !";
    return false;
}

void CTcpServer::stopListen(){
    this->close();
}

void CTcpServer::incomingConnection(qintptr socketDescriptor){
    qDebug()<<"new Connection:"<<socketDescriptor;
    CTcpThread* pThread = new CTcpThread(socketDescriptor, 0);
    //服务端向下发送报文
    connect(this,&CTcpServer::writeData,pThread,&CTcpThread::writeData);
    //接收各个客户端发送来的报文
    connect(pThread,&CTcpThread::sendDataToQueue,this,&CTcpServer::sendDataToQueue);
    connect(pThread,SIGNAL(disconnected(qintptr)),this,SLOT(slotDisconnected(qintptr)));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));

    pThread->start();

    mThreadMap.insert(socketDescriptor,pThread);
}

void CTcpServer::slotDisconnected(qintptr handle){
    QMap<qintptr,CTcpThread*>::iterator it = mThreadMap.find(handle);
    if( it != mThreadMap.end() ){
        mThreadMap.remove(it.key());
        qDebug()<<it.key()<<"is Removed,CTcpServer::Left="<<mThreadMap.size();
    }
}

/*---------------------------------CTcpSocket---------------------------------*/
CTcpSocket::CTcpSocket(qintptr socketDescriptor, QObject *parent):QTcpSocket(parent){
    mSocketDescriptor = socketDescriptor;
    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

//slots
void CTcpSocket::readData(){
    if( this->bytesAvailable() > sizeof(FrameHead) ){
        QByteArray rcvAry = this->readAll();
        parseDatagram(rcvAry);
    }
}

void CTcpSocket::slotDisconnected(){
    //发送断开连接信号
    emit disconnected(mSocketDescriptor);
    qDebug()<<mSocketDescriptor<<"断开连接...";
}


void CTcpSocket::writeData(unsigned char* sendBuf,int nSendLen,qintptr handle){
    if( mSocketDescriptor == handle ){
        //未作转码操作
        this->write((char*)sendBuf,nSendLen);
    }
}


void CTcpSocket::parseDatagram(QByteArray rcvAry){
    //将获取到的报文添加到缓存中
    mCacheAry.append(rcvAry);
    //计算缓存长度
    int nCacheLen = mCacheAry.length();
    while( nCacheLen > 0 ){
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
        if( true == bFindHead && true ==bFindTail ){
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

void CTcpSocket::switchDatagram(unsigned char* cRcvBuf,int nTotalLen){
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
    emit sendDataToQueue((unsigned char*)&head,sizeof(FrameHead),mSocketDescriptor);
}

