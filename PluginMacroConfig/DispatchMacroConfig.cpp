#include "DispatchMacroConfig.h"
#include <QDomDocument>
#include <QDebug>
#include <QFile>

DispatchMacroConfig::DispatchMacroConfig(QObject *parent) :
    QObject(parent),
    m_nRadarId(-1)
{

}

bool DispatchMacroConfig::startDispatch(QString strFilePath)
{
    if( initSubMacroConfigList(strFilePath) ){
        dispatchFirst();//发送第一条命令
        return true;
    }

    return false;
}

bool DispatchMacroConfig::initSubMacroConfigList(QString strFilePath)
{
    QFile file(strFilePath);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<<"Open XML Failed,FilePath:"<<strFilePath;
        return false;
    }

    QDomDocument doc;
    QString error = "";
    int row = 0, column = 0;
    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return false;
    }
    file.close();

    m_smcList.clear();
    QDomElement root = doc.documentElement();
    if( QStringLiteral("设备配置参数")==root.tagName() ){
        QDomElement devNode=root.firstChildElement(QStringLiteral("设备"));
        while( !devNode.isNull() ){
            //设备
            QString strSubDevId=devNode.attribute("subDevId");

            SubMacroConfig smc;
            smc.strSubDevId=strSubDevId;

            QDomElement cmdNode=devNode.firstChildElement(QStringLiteral("命令"));
            while (!cmdNode.isNull()) {
                //命令
                QString strCMDTag=cmdNode.attribute(QStringLiteral("cmd"));

                CMD_PV cpv;
                cpv.strCMDTag=strCMDTag;
                //参数
                QDomElement paraNode=cmdNode.firstChildElement(QStringLiteral("参数"));
                while (!paraNode.isNull()) {
                    QString strParaId=paraNode.attribute(QStringLiteral("paraId")).trimmed();
                    QString strValue=paraNode.attribute(QStringLiteral("value")).trimmed();
                    paraNode = paraNode.nextSiblingElement(QStringLiteral("参数"));

                    ParaID_Value pv;
                    pv.strParaId=strParaId;
                    pv.strParaVal=strValue;
                    cpv.pvList.push_back(pv);
                }
                cmdNode = cmdNode.nextSiblingElement(QStringLiteral("命令"));
                smc.cpvList.push_back(cpv);
            }
            devNode = devNode.nextSiblingElement(QStringLiteral("设备"));

            if( smc.cpvList.size()>0 ){//过滤无命令的设备
                m_smcList.push_back(smc);
            }
        }
    }
    return true;
}

void DispatchMacroConfig::dispatchFirst()
{
    if( m_smcList.size()>0 ){
        if( m_smcList.first().cpvList.size()>0 ){
            QString strSubDevId=m_smcList.first().strSubDevId;
            QString strCMDTag=m_smcList.first().cpvList.first().strCMDTag;
            QStringList keyValueList;
            foreach (ParaID_Value pv, m_smcList.first().cpvList.first().pvList) {
                keyValueList<<pv.strParaId+"|"+pv.strParaVal;
            }
            QString strKeyValues=keyValueList.join(",");
            emit publishMacroCfg(strSubDevId,strCMDTag,strKeyValues);
        }
        else{
            m_smcList.pop_front();
        }
    }
}

void DispatchMacroConfig::onDispatchNext()
{
    if( m_smcList.size()>0 ){
        if( m_smcList.first().cpvList.size()>0 ){
            //先清除已发送的宏配置
            m_smcList.first().cpvList.pop_front();
            if( m_smcList.first().cpvList.size()>0 ){

                QString strSubDevId=m_smcList.first().strSubDevId;
                QString strCMDTag=m_smcList.first().cpvList.first().strCMDTag;
                QStringList keyValueList;
                foreach (ParaID_Value pv, m_smcList.first().cpvList.first().pvList) {
                    keyValueList<<pv.strParaId+"|"+pv.strParaVal;
                }
                QString strKeyValues=keyValueList.join(",");
                emit publishMacroCfg(strSubDevId,strCMDTag,strKeyValues);
            }
        }
        else{
            m_smcList.pop_front();
            if( m_smcList.size()<=0 ){
                emit dispatchFinished();
            }
        }
    }
    else{
        emit dispatchFinished();
    }
}

void DispatchMacroConfig::onTimer()
{
    onDispatchNext();
}
