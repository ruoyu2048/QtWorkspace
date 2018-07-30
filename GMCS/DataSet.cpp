#include "DataSet.h"
#include <QMap>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>
#include "MacroDef.h"


DataSet globalDS;

DataSet::DataSet(QObject *parent) :
    QObject(parent)
{
    m_root = new TreeItem;
}

DataSet::~DataSet()
{
    qDeleteAll(m_vHostOnlineInfo);
}


bool DataSet::OpenXMLFile(QString strFileName)
{
    if(strFileName.isEmpty())
        return false;

    QFile file(strFileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString errorInfo=QString("LayoutCfg.xml 文件打开失败！\n 错误信息：%1").arg(file.errorString());
        MessageBoxEx(errorInfo,true);
        return false;
    }

    QString strError;
    int row=0,column=0;
    if(!m_DomConnment.setContent(&file,&strError,&row,&column))
    {
        QString errorInfo=QString("Parse file failed at line row and column %1 %2").arg(QString::number(row,10)).arg(QString::number(column,10));
        MessageBoxEx(errorInfo,true);
        file.close();
        return false;
    }
    if(m_DomConnment.isNull())
    {
        QString errorInfo = "Document is null!";
        MessageBoxEx(errorInfo,true);
        file.close();
        return false;
    }
    file.close();
    return true;
}

void DataSet::SetLayoutCfgFilePath(QString strFilePath)
{
    m_strLayoutCfgFilePath=strFilePath;
}

void DataSet::SetAttributeValue(QString strXMLFile,QString strItemId,QString strAttr,QString strVal)
{
    m_XMLMutex.lock();
    if(OpenXMLFile(strXMLFile) && !strItemId.isEmpty())
    {
        QDomElement root=m_DomConnment.documentElement();

        bool bFind = false;
        //Position
        QDomElement position=root.firstChildElement();
        while (!position.isNull() && !bFind)
        {
            QString strPosId=strItemId.left(2);
            QString curPosId=position.attributeNode("id").value();
            if(strPosId==curPosId)
            {
                if(curPosId == strItemId)
                {
                    position.setAttribute(strAttr,strVal);
                    bFind = true;
                    break;
                }
                //Group
                QDomElement group=position.firstChildElement();
                while (!group.isNull() && !bFind)
                {
                    QString strGroupId=strItemId.left(4);
                    QString curGroupId=group.attributeNode("id").value();
                    if(strGroupId==curGroupId)
                    {
                        if(curGroupId == strItemId)
                        {
                            group.setAttribute(strAttr,strVal);
                            bFind = true;
                            break;
                        }
                        //Model
                        QDomElement model=group.firstChildElement();
                        while (!model.isNull() && !bFind)
                        {
                            QString strModelId=strItemId.left(6);
                            QString curModelId=model.attributeNode("id").value();
                            if(strModelId==curModelId)
                            {
                                if(curModelId == strItemId)
                                {
                                    model.setAttribute(strAttr,strVal);
                                    bFind = true;
                                    break;
                                }
                                //Host
                                QDomElement host=model.firstChildElement();
                                while (!host.isNull() && !bFind)
                                {
                                    if(host.attributeNode("id").value()==strItemId)
                                    {
                                        host.setAttribute(strAttr,strVal);
                                        bFind = true;
                                        break;
                                    }
                                    host=host.nextSiblingElement();
                                }
                            }
                            model=model.nextSiblingElement();
                        }
                    }
                    group=group.nextSiblingElement();
                }
            }
            position=position.nextSiblingElement();
        }

        QFile file(strXMLFile);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QString errorInfo = QString("Open XML file failed!\n%1").arg(file.errorString());
            MessageBoxEx(errorInfo);
            return ;
        }

        QTextStream output(&file);
        m_DomConnment.save(output,4,QDomNode::EncodingFromTextStream);
        file.close();
    }
    m_XMLMutex.unlock();
}

QString DataSet::GetAttributeValue(QString strXMLFile,QString strItemId,QString strAttr)
{
    QString strValue="";
    m_XMLMutex.lock();
    if(OpenXMLFile(strXMLFile) && !strItemId.isEmpty())
    {
        QDomElement root=m_DomConnment.documentElement();

        bool bFind = false;
        //Position
        QDomElement position=root.firstChildElement();
        while (!position.isNull() && !bFind)
        {
            QString strPosId=strItemId.left(2);
            QString curPosId=position.attributeNode("id").value();
            if(strPosId==curPosId)
            {
                if(curPosId==strItemId)
                {
                    strValue=position.attributeNode(strAttr).value();
                    bFind = true;
                    break;
                }
                //Group
                QDomElement group=position.firstChildElement();
                while (!group.isNull() && !bFind)
                {
                    QString strGroupId=strItemId.left(4);
                    QString curGroupId=group.attributeNode("id").value();
                    if(strGroupId==curGroupId)
                    {
                        if(curGroupId==strItemId)
                        {
                            strValue=group.attributeNode(strAttr).value();
                            bFind = true;
                            break;
                        }
                        //Model
                        QDomElement model=group.firstChildElement();
                        while (!model.isNull() && !bFind)
                        {
                            QString strModelId=strItemId.left(6);
                            QString curModelId=model.attributeNode("id").value();
                            if(strModelId==curModelId)
                            {
                                if(curModelId==strItemId)
                                {
                                    strValue=model.attributeNode(strAttr).value();
                                    bFind = true;
                                    break;
                                }
                                //Host
                                QDomElement host=model.firstChildElement();
                                while (!host.isNull() && !bFind)
                                {
                                    if(host.attributeNode("id").value()==strItemId)
                                    {
                                        strValue=host.attributeNode(strAttr).value();
                                        bFind = true;
                                        break;
                                    }
                                    host=host.nextSiblingElement();
                                }
                            }
                            model=model.nextSiblingElement();
                        }
                    }
                    group=group.nextSiblingElement();
                }
            }
            position=position.nextSiblingElement();
        }
    }
    m_XMLMutex.unlock();
    return strValue;
}

TreeItem* DataSet::GetRoot()
{
    return m_root;
}

TreeItem* DataSet::FindInAllItems(QString itemId)
{
    TreeItem* pItem = NULL;
    m_AllItemsMutex.lock();
    QMap<QString,TreeItem*>::iterator it = m_AllItems.find(itemId);
    if(it!=m_AllItems.end())
    {
        pItem = it.value();
    }
    m_AllItemsMutex.unlock();
    return pItem;
}

TreeItem* DataSet::FindHostItem(QString itemId)
{
    TreeItem* pItem = NULL;
    m_AllHostsMutes.lock();
    QMap<QString,TreeItem*>::iterator it = m_AllHosts.find(itemId);
    if(it!=m_AllHosts.end())
    {
        pItem = it.value();
    }
    m_AllHostsMutes.unlock();
    return pItem;
}

QString DataSet::FindHostID(QString itemIP,QString transferHostIP,bool bInface)
{
    QString strHostId="";
    m_AllHostsMutes.lock();
    //"0名称"<<"1位置编码"<<"2状态"<<"3更新时间"<<"4MAC"<<"5模拟指挥网"<<"6仿真对抗网"<<"7中转主机IP"<<"8本机类型";
    int iColIndex = 6;
    if(bInface)
    {
        iColIndex = 5;
    }
    QMap<QString,TreeItem*>::iterator it = m_AllHosts.begin();
    for(;it!=m_AllHosts.end();it++)
    {
        //获取中转主机IP地址
        QString curItemIP = it.value()->GetData(iColIndex).toString();
        QString curTransferHostIP = it.value()->GetData(7).toString();
        if( (curItemIP == itemIP)  &&  (curTransferHostIP==transferHostIP) )
        {
            strHostId = it.value()->GetData(1).toString();
            break;
        }
    }
    m_AllHostsMutes.unlock();
    return strHostId;
}

void DataSet::SetupModelData(QString strXMLFile)
{
    if(NULL!=m_root)
    {
        if(OpenXMLFile(strXMLFile))
        {
            m_AllItems.clear();
            QDomElement root=m_DomConnment.documentElement();
            QString curTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            //Position
            QDomElement position=root.firstChildElement();
            while (!position.isNull())
            {
                QString positionId=position.attributeNode("id").value();
                QString positionName=position.attributeNode("name").value();
                QVector<QVariant>positionData;
                positionData<<positionName<<positionId;
                TreeItem* positionItem = new TreeItem(positionData,m_root);
                positionItem->SetCheckable( true );
                positionItem->SetCheckState(Qt::Unchecked);
                m_root->AppendChild(positionItem);

                m_AllItems.insert(positionId,positionItem);
                //Group
                QDomElement group=position.firstChildElement();
                while (!group.isNull())
                {
                    QString groupId=group.attributeNode("id").value();
                    QString groupName=group.attributeNode("name").value();
                    QVector<QVariant>groupData;
                    groupData<<groupName<<groupId;
                    TreeItem* groupItem = new TreeItem(groupData,positionItem);
                    groupItem->SetCheckable( true );
                    groupItem->SetCheckState(Qt::Unchecked);
                    positionItem->AppendChild(groupItem);

                    m_AllItems.insert(groupId,groupItem);
                    //Model
                    QDomElement model=group.firstChildElement();
                    while (!model.isNull())
                    {
                        QString modelId=model.attributeNode("id").value();
                        QString modelName=model.attributeNode("name").value();
                        QVector<QVariant>modelData;
                        modelData<<modelName<<modelId;
                        TreeItem* modelItem = new TreeItem(":/icon/2_1red.png",modelData,groupItem);
                        modelItem->SetCheckable( true );
                        modelItem->SetCheckState(Qt::Unchecked);
                        groupItem->AppendChild(modelItem);

                        m_AllItems.insert(modelId,modelItem);
                        m_AllModels.insert(modelId,modelItem);
                        //Host
                        QDomElement host=model.firstChildElement();
                        while (!host.isNull())
                        {
                            QString hostId=host.attributeNode("id").value();
                            QString hostName=host.attributeNode("name").value();
                            QString hostMac=host.attributeNode("mac").value();
                            QString hostAIP=host.attributeNode("aIP").value();
                            QString hostCIP=host.attributeNode("cIP").value();
                            QString hostSIP=host.attributeNode("sIP").value();
                            QString hostType=host.attributeNode("type").value();
                            QVector<QVariant>hostData;
                            hostData<<hostName<<hostId<<"关机"<<curTime<<hostMac<<hostAIP<<hostCIP<<hostSIP<<hostType;
                            TreeItem* hostItem = new TreeItem(":/icon/2_1red.png",hostData,modelItem);
                            hostItem->SetCheckable( true );
                            hostItem->SetCheckState(Qt::Unchecked);
                            modelItem->AppendChild(hostItem);

                            m_AllItems.insert(hostId,hostItem);
                            m_AllHosts.insert(hostId,hostItem);
                            if(hostType=="中转主机")
                            {
                                //将中转主机的的基本信息添加到在线检测队列
                                HostOnlineInfo* pHostOnlineInfo=new HostOnlineInfo;
                                pHostOnlineInfo->strHostIP=hostAIP;
                                pHostOnlineInfo->strHostId=hostId;
                                pHostOnlineInfo->iState=0;
                                pHostOnlineInfo->iScanCnt=2;
                                m_vHostOnlineInfo.push_back(pHostOnlineInfo);
                            }
                            host=host.nextSiblingElement();
                        }
                        model=model.nextSiblingElement();
                    }
                    group=group.nextSiblingElement();
                }
                position=position.nextSiblingElement();
            }
        }
    }
}

void DataSet::MessageBoxEx(QString errorInfo,bool bQuit)
{
    int ret=QMessageBox::warning(NULL,"提示",errorInfo,QMessageBox::Warning,QMessageBox::Ok);
    if(QMessageBox::Cancel == ret && bQuit)
    {
        emit quitApplication();
    }
}
