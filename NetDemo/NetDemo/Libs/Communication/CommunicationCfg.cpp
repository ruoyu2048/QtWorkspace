#include "CommunicationCfg.h"
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CommunicationCfg::CommunicationCfg()
{

}

void CommunicationCfg::parseCommCfgFile(QString strCfgPath)
{
    QFile cfgFile(strCfgPath);
    if( !cfgFile.open(QIODevice::ReadOnly) ){
        qDebug() << "could't open communication config json";
        return;
    }

    QByteArray cfgData = cfgFile.readAll();
    cfgFile.close();

    QJsonParseError jsonError;
    QJsonDocument cfgJsonDoc(QJsonDocument::fromJson(cfgData,&jsonError));

    if( jsonError.error != QJsonParseError::NoError ){
        qDebug() << "json error!";
        return;
    }

    QJsonObject cfgObj = cfgJsonDoc.object();
    QJsonArray commAry = cfgObj["connnection_config"].toArray();
    if( commAry.size() == 1 ){
        QJsonObject commObj = commAry.at(0).toObject();
        strCommType = commObj["comm_type"].toString();
        strCommPara = commObj["comm_para"].toString();
        strCommOther = commObj["comm_other"].toString();
    }
    if( "TcpServer" == strCommType )
        commType = CommType::TcpServer;
    if( "TcpClient" == strCommType )
        commType = CommType::TcpClient;
    if( "UDP" == strCommType )
        commType = CommType::UDP;
    if( "Serial" == strCommType )
        commType = CommType::Serial;
}
