#include "Communication.h"
#include "CTcpServer.h"
#include "CTcpClient.h"

Communication::Communication(QObject *parent) : QObject(parent)
{

}

bool Communication::startCommunication(CommType commType,QStringList cfg){

    return true;
}

bool Communication::startTcpServer(QString strIP,quint16 nPort){
    if( NULL == m_pTS )
        m_pTS = new CTcpServer(this);
    return m_pTS->StartListen(strIP,nPort);
}

bool Communication::startTcpClient(QString strIP,quint16 nPort){
    if( NULL == m_pTS )
        m_pTC = new CTcpClient(this);
    qDebug()<<"Tcp Client Created successfully..";
    return m_pTC->ConnectToHost(strIP,nPort);
}
