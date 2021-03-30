#include "timethread.h"
#include <QDateTime>
#include <QByteArray>
#include <QDataStream>

TimeThread::TimeThread(qintptr socketDescriptor,QObject *parent)
    :QThread(parent),socketDescriptor(socketDescriptor),runFlag(true)
{
}

void TimeThread::run()
{
    QTcpSocket tcpSocket;
    if(!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }
    while(runFlag)
    {
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_11);
        QDateTime dateTime=QDateTime::currentDateTime();
        qDebug()<<dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
        uint time2u = dateTime.toTime_t();
        out<<time2u;
        //out<<dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz").toLatin1();

        if( -1 != tcpSocket.write(block)){
            tcpSocket.flush();
        }
        else{
            qDebug()<<tcpSocket.errorString();
        }

        QThread::msleep(1000);
    }
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}
