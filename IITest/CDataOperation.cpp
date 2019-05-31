#include "CDataOperation.h"

CDataOperation::CDataOperation(QObject *parent) : QObject(parent)
{
    m_pCommu=new Communication(this);
}

bool CDataOperation::startDataOperation(CommunicationCfg commCfg)
{
    return m_pCommu->startCommunication(commCfg);
}

void CDataOperation::stopDataOperation()
{
    m_pCommu->stopCommunication();
}

bool CDataOperation::sendData(QByteArray sendAry)
{
    return m_pCommu->sendData(sendAry);
}

void CDataOperation::onReadData(QByteArray recvAry)
{
    //将获取到的报文添加到缓存中
    m_cacheAry.append(recvAry);
    while( m_cacheAry.length() >= 8 ){
        int nHeadPos = m_cacheAry.indexOf(FRAME_HEAD);
        if( nHeadPos >=0 ){
                //必须保证报尾在报头后面
                int nTailPos = m_cacheAry.indexOf(FRAME_END,nHeadPos);
                if( nTailPos >=0 ){//有头有尾
                    QByteArray encodeAry = m_cacheAry.mid(nHeadPos,nTailPos-nHeadPos+1);
                    emit receivedEncodeAry(encodeAry);
                    //移除已解析的报文
                    m_cacheAry.remove( 0, nTailPos+1 );
                }
                else{//有头无尾,
                    break;
                }
        }
        else {
            int nTailPos = m_cacheAry.indexOf(FRAME_END);
            if( nTailPos >= 0 ){//无头有尾
                m_cacheAry.remove( 0,nTailPos+1 );
            }
            else{//无头无尾
                m_cacheAry.clear();
            }
        }
    }
}
