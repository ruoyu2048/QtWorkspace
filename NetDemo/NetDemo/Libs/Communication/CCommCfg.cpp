#include "CCommCfg.h"
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CCommCfg::CCommCfg()
{

}

void CCommCfg::parseCommCfgFile(QString strCfgPath)
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
    if( strCommType == "TcpServr" )
        commType == CommType::TcpServer;
    if( strCommType == "TcpClient" )
        commType == CommType::TcpClient;
    if( strCommType == "UDP" )
        commType == CommType::UDP;
    if( strCommType == "Serial" )
        commType == CommType::Serial;
}
