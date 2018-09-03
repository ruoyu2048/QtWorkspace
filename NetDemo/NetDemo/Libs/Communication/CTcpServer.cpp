#include "CTcpServer.h"
#include "CTcpThread.h"
#include "DataStruct.h"
#include "CDataPacket.h"

CTcpServer::CTcpServer(QObject *parent) : QTcpServer(parent){
    qRegisterMetaType<qintptr>("qintptr");
}

bool CTcpServer::startListen(QString strUrl){
    QStringList netInfo=strUrl.split(":");
    if( netInfo.size() >=2 ){
        QHostAddress hostAddr(netInfo.at(0));
        if( this->listen(hostAddr,netInfo.at(1).toInt()) ){
            qDebug()<<"Server Started Sucessfully !";
            return true;
        }
        qDebug()<<"Tcp Server Started Failed !";
        return false;
    }
    qDebug()<<"The TcpServer listen info is not commplete,cfg:"<<strUrl;
    return false;
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
    CTcpThread* pThread = new CTcpThread(socketDescriptor, 0);
    //将客户端报文发送到上层消息队列
    connect(pThread,SIGNAL(registerDstId(QSet<quint8>,qintptr)),this,SLOT(registerDstId(QSet<quint8>,qintptr)));
    connect(pThread,SIGNAL(sendDataToQueue(CDataPacket*)),this,SIGNAL(sendDataToQueue(CDataPacket*)));

    connect(pThread,SIGNAL(disconnected(qintptr)),this,SLOT(slotDisconnected(qintptr)));
    connect(pThread, SIGNAL(finished()), pThread, SLOT(deleteLater()));

    pThread->start();

    mThreadMap.insert(socketDescriptor,pThread);
}

void CTcpServer::registerDstId(QSet<quint8>dstIDs,qintptr socketDesc ){
    foreach (quint8 dstId, dstIDs) {
        mDstIdMap.insert(dstId,socketDesc);
        qDebug()<<"【服务器客户端注册中心】客户端注册标识:"<<socketDesc<<"订阅消息信宿:"<<dstId;
    }
}

void CTcpServer::dispatchData( CDataPacket* dataPkt ){
    if( NULL != dataPkt ){
        QMap<quint8,qintptr>::iterator itDstId = mDstIdMap.find(dataPkt->msgType);
        if( itDstId != mDstIdMap.end() ){
            QMap<qintptr,CTcpThread*>::iterator itThread = mThreadMap.find(itDstId.value());
            if( itThread != mThreadMap.end() ){
                qDebug()<<"【服务端报文发布中心】客户端注册标识:"<<itDstId.value()<<"发布报文类型:"<<itDstId.key();
                itThread.value()->writeData(dataPkt);
            }
        }
    }
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

bool CTcpSocket::registClientInfo(CDataPacket* dataPkt ){
    if( NULL != dataPkt && dataPkt->msgType == 0xFF ){
        int nTypes = dataPkt->msgLen;
        for(int i=0;i<nTypes;i++){
            quint8 dstId = dataPkt->msgData.at(i);
            mDstIdSet.insert(dstId);
        }
        emit registerDstId(mDstIdSet,mSocketDescriptor);

        return false;
    }
    return true;
}

//slots
void CTcpSocket::readData(){
    if( this->bytesAvailable() > 0 ){
        QByteArray rcvAry = this->readAll();
        parseDatagram(rcvAry);
    }
}

void CTcpSocket::slotDisconnected(){
    //发送断开连接信号
    emit disconnected(mSocketDescriptor);
    qDebug()<<mSocketDescriptor<<"断开连接...";
}

void CTcpSocket::writeData(CDataPacket* dataPkt){
    if( NULL != dataPkt ){
        this->write(dataPkt->packetToBytes());
    }
}

void CTcpSocket::parseDatagram(QByteArray rcvAry){
    //将获取到的报文添加到缓存中
    mCacheAry.append(rcvAry);
    while( mCacheAry.length() >= 8 ){
        int nHeadPos = mCacheAry.indexOf(0xAA);
        if( nHeadPos >=0 ){
                //必须保证报尾在报头后面
                int nTailPos = mCacheAry.indexOf(0xA5,nHeadPos);
                if( nTailPos >=0 ){//有头有尾
                    //quint16 nDataLen = ((uchar)mCacheAry[4]<<8) + (uchar)mCacheAry[5];
                    //QByteArray dataAry = mCacheAry.mid(nHeadPos,4+2+nDataLen+2);
                    QByteArray dataAry = mCacheAry.mid(nHeadPos,nTailPos-nHeadPos+1);
                    CDataPacket* dataPkt = new CDataPacket();
                    dataPkt->bytesToPacket( dataAry );
                    if( registClientInfo(dataPkt) ){
                        emit sendDataToQueue(dataPkt);
                    }
                    //移除已解析的报文
                    mCacheAry.remove( 0, nTailPos+1 );
                }
                else{//有头无尾,
                    break;
                }
        }
        else {
            int nTailPos = mCacheAry.indexOf(0xA5);
            if( nTailPos >= 0 ){//无头有尾
                mCacheAry.remove( 0,nTailPos+1 );
            }
            else{//无头无尾
                mCacheAry.clear();
            }
        }
    }

//    //计算缓存长度
//    int nCacheLen = mCacheAry.length();
//    while( nCacheLen > 0 ){
//        //解析缓存
//        int nMaxRcvBufLen = nCacheLen;
//        unsigned char cRcvBuf[COMMAXLEN] = {'0'};
//        if( nCacheLen >= COMMAXLEN ){
//            nMaxRcvBufLen = COMMAXLEN;
//        }
//        memmove(cRcvBuf,(unsigned char*)mCacheAry.data(),nMaxRcvBufLen);
//        /*---------------------------------报文数据完整性验证---------------------------------*/
//        //先找帧头
//        bool bFindHead = true;
//        if( 0xAA != cRcvBuf[0] ){//帧头不正确
//            qDebug()<<"帧头不正确 [Head:"<<cRcvBuf[0]<<"]";
//            bFindHead = false;
//        }
//        //再找帧尾
//        int nTotalLen = 0;
//        bool bFindTail = false;
//        for( ;nTotalLen<nMaxRcvBufLen;nTotalLen++ ){
//            if( 0xA5 == cRcvBuf[nTotalLen] ){
//                ++nTotalLen;
//                bFindTail = true;
//                break;
//            }
//        }

//        //有效报文(有头有尾)
//        if( true == bFindHead && true ==bFindTail ){
//            //计算校验位
////            unsigned char cCheck = cRcvBuf[1];
////            for(int i=2;i<nTotalLen-2;i++){
////                cCheck = cCheck^cRcvBuf[i];
////            }

//            int nStartPos = 4;
//            unsigned char cCheck = cRcvBuf[nStartPos];
//            for(int i=nStartPos+1;i<nTotalLen-2;i++){
//                cCheck += cRcvBuf[i]&0xFF;
//            }
//            cCheck = cCheck&0xFF;

//            //比较校验位
//            if( cCheck != cRcvBuf[nTotalLen-2] ){//校验位判断
//                qDebug()<<"校验位错误 [cCheck:"<<cCheck<<"SrcCheck:"<<cRcvBuf[nTotalLen-2]<<"]";
//                qDebug()<<"mCacheAry:"<<mCacheAry<<"\n";
//                mCacheAry.remove(0,nTotalLen);//将错误信息丢弃
//                nCacheLen = mCacheAry.length();
//                continue;
//            }
//            /*---------------------------------解析报文---------------------------------*/
//            switchDatagram(cRcvBuf, nTotalLen);
//            //将解析过的报文数据从缓存中清除
//            mCacheAry.remove(0,nTotalLen);
//            //重新计算缓存中的报文数据长度
//            nCacheLen = mCacheAry.length();
//        }
//        else
//        {
//            //有头无尾，则跳过
//            if( true == bFindHead && false == bFindTail ){
//                break;
//            }
//            else{
//                //无头无尾、无头有尾，则丢弃
//                QByteArray invalidData = mCacheAry.mid(0,nTotalLen);
//                qDebug()<<"InvalidDataLen:"<<nTotalLen<<"InvalidData:"<<invalidData;
//                //将无效的报文数据从缓存中丢弃
//                mCacheAry.remove(0,nTotalLen);
//                //重新计算缓存中的报文数据长度
//                nCacheLen = mCacheAry.length();
//                qDebug()<<"CurCacheLen:"<<nCacheLen<<"CurCacheData:"<<mCacheAry<<"\n";
//                continue;
//            }
//        }
//    }
}

void CTcpSocket::switchDatagram(unsigned char* cRcvBuf,int nTotalLen){
//    //帧头
//    FrameHead head;
//    memmove((unsigned char*)&head,cRcvBuf,sizeof(FrameHead));
//    //报头信息(帧头、目的地址、源地址、类型)
//    //qDebug()<<head.cHead<<head.cDesAdd<<head.cSrcAdd<<head.cType;

//    CDataPacket dataPkt;
//    dataPkt.msgType = head.cType;
//    dataPkt.msgData.append((char*)cRcvBuf);
//    emit sendDataToQueue(&dataPkt);

//    //信息区
//    short nInfoLen = 0;//信息长度=长度位(2)+信息区位长度(N)
//    unsigned char cInfoBuf[COMMAXLEN] = {'0'};//解码后报文=转码后的长度+信息区
//    RecvBufChange(cRcvBuf,cInfoBuf,nTotalLen,nInfoLen);

//    switch (head.cType) {
//    case 0x32:{
//        MidInfo midInfo;
//        memmove((unsigned char*)&midInfo,cInfoBuf+sizeof(short),sizeof(MidInfo));
//        qDebug()<<midInfo.cLDState<<midInfo.cWorkMode<<midInfo.cPulseInput<<midInfo.cOLMid<<midInfo.cDSMid;
//    }
//        break;
//    case 0x33:
//        break;
//    }

//    //eg:
//    //emit sendDataToQueue((unsigned char*)&head,sizeof(FrameHead),mSocketDescriptor);
}

