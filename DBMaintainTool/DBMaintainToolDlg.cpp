#include "DBMaintainToolDlg.h"
#include "ui_DBMaintainToolDlg.h"
#include <QFile>
#include <QtTest/QtTest>
#include <QTextStream>
#include <QMessageBox>
#include <QSqlQuery>
#include "GlobalDef.h"

DBMaintainToolDlg::DBMaintainToolDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DBMaintainToolDlg)
{
    ui->setupUi(this);
    InitLayout();
}

DBMaintainToolDlg::~DBMaintainToolDlg()
{
    delete ui;
    m_vSimulatorInfo.clear();
    if(m_bIsConnect)
    {
        theDB.CloseDatabase();
    }
}

void DBMaintainToolDlg::InitLayout()
{
    m_bSimCode=m_bSimNode=m_bIsConnect=false;
    m_pGroupBox = new QGroupBox(tr("数据表"));
    m_pCBSimCode=new QCheckBox();
    m_pCBSimCode->setText(tr("模拟器编码表"));
    m_pCBSimNode=new QCheckBox();
    m_pCBSimNode->setText(tr("模拟器仿真节点计算机信息表"));
    m_pVBoxlayout = new QVBoxLayout();
    m_pVBoxlayout->addWidget(m_pCBSimCode);
    m_pVBoxlayout->addWidget(m_pCBSimNode);
    m_pGroupBox->setLayout(m_pVBoxlayout);
    m_pBtnOutput=new QPushButton();
    m_pBtnOutput->setText(tr("导出数据"));
    m_pBtnInput=new QPushButton();
    m_pBtnInput->setText(tr("导入数据"));

    m_pGridLayout=new QGridLayout(this);
    m_pGridLayout->addWidget(m_pGroupBox,0,0,9,6);
    m_pGridLayout->addWidget(m_pBtnOutput,10,1,2,1);
    m_pGridLayout->addWidget(m_pBtnInput,10,4,2,1);

    connect(m_pCBSimCode,SIGNAL(clicked()),this,SLOT(CBSimCode()));
    connect(m_pCBSimNode,SIGNAL(clicked()),this,SLOT(CBSimNode()));
    connect(m_pBtnOutput,SIGNAL(clicked()),this,SLOT(BtnOutput()));
    connect(m_pBtnInput,SIGNAL(clicked()),this,SLOT(BtnInput()));

    ConnectToDB();
}

void DBMaintainToolDlg::ConnectToDB()
{
    theDB.GetDatabaseInfo();
    theDB.GetDatasheetName();
    m_bIsConnect=theDB.ConnectDatabase();
    if(!m_bIsConnect)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("数据库连接失败，请检查配置信息!"));
        msgBox.exec();
    }
}

void DBMaintainToolDlg::ReadConfigFromTxt()
{
    QString strCfgPath=theBG.GetAppCurPath();
    strCfgPath.append(tr("/模拟器编码表.txt"));
    QFile file(strCfgPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("模拟器编码表.txt 文件打开失败！"));
        msgBox.exec();
        return;
    }

    m_vSimulatorInfo.clear();
    QTextStream inSimCode(&file);
    while (!inSimCode.atEnd())
    {
        QString line = inSimCode.readLine();
        if(line.contains("//",Qt::CaseSensitive))
            continue;
        QStringList strItems=line.split("\t");
        SimulatorInfo* pSimulatorInfo=new SimulatorInfo;
        pSimulatorInfo->cName=strItems.at(0);
        pSimulatorInfo->iSimCode=strItems.at(1).toInt();
        pSimulatorInfo->cLocation=strItems.at(2);
        pSimulatorInfo->cGroup=strItems.at(3);
        pSimulatorInfo->iSubNodeNum=0;
        m_vSimulatorInfo.append(pSimulatorInfo);
    }

    strCfgPath.clear();
    strCfgPath=theBG.GetAppCurPath();
    strCfgPath.append(tr("/模拟器节点计算机信息表.txt"));
    QFile fileSimNodeInfo(strCfgPath);
    if (!fileSimNodeInfo.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("模拟器节点计算机信息表.txt 文件打开失败！"));
        msgBox.exec();
        return;
    }

    QTextStream in(&fileSimNodeInfo);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(line.contains("//",Qt::CaseSensitive))
            continue;
        QStringList strItems=line.split("\t");
        int iSimCode=strItems.at(5).toInt();
        SimulatorInfo* pSimulatorInfo=FindSimulatorInfo(iSimCode);
        if(NULL!=pSimulatorInfo)
        {
            int iSubNodeIndex=pSimulatorInfo->iSubNodeNum;
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].cName=strItems.at(0);
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].cMac=strItems.at(1);
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].cIP=strItems.at(2);
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].cInIP=strItems.at(3);
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].cServerIP=strItems.at(4);
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].iSimCode=strItems.at(5).toInt();
            pSimulatorInfo->simNodeInfo[iSubNodeIndex].cPCType=strItems.at(6);
            ++(pSimulatorInfo->iSubNodeNum);
        }
    }
}

void DBMaintainToolDlg::ReadConfigFromDB()
{
    GetSimulatorInfoFromDB();
    GetSimNodePCInfoFromDB();
}

void DBMaintainToolDlg::GetSimulatorInfoFromDB()
{
    m_vSimulatorInfo.clear();
    QString strQuery = QString("SELECT * FROM %1").arg(theDB.m_strSimulatorCodeTable);
    QSqlQuery query;
    query.prepare(strQuery);
    if(query.exec())
    {
       while(query.next())
       {
           SimulatorInfo* pSimulatorInfo=new SimulatorInfo;
           pSimulatorInfo->cName=query.value(0).toString();
           pSimulatorInfo->iSimCode=query.value(1).toInt();
           pSimulatorInfo->cLocation=query.value(2).toString();
           pSimulatorInfo->cGroup=query.value(3).toString();
           pSimulatorInfo->iSubNodeNum=0;
           m_vSimulatorInfo.append(pSimulatorInfo);
       }
       query.clear();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(tr("模拟器编码表 打开失败！"));
        msgBox.exec();
    }
}

void DBMaintainToolDlg::GetSimNodePCInfoFromDB()
{
   QString strQuery = QString("SELECT * FROM %1").arg(theDB.m_strSimNodePCInfoTable);
   QSqlQuery query;
   query.prepare(strQuery);
   if(query.exec())
   {
       while(query.next())
       {
            SimulatorInfo* pSimulatorInfo=FindSimulatorInfo(query.value(5).toInt());
            if(NULL != pSimulatorInfo)
            {
                int iSubNodeNum=pSimulatorInfo->iSubNodeNum;
                pSimulatorInfo->simNodeInfo[iSubNodeNum].cName=query.value(1).toString();
                pSimulatorInfo->simNodeInfo[iSubNodeNum].cMac=query.value(2).toString();
                pSimulatorInfo->simNodeInfo[iSubNodeNum].cIP=query.value(3).toString();
                pSimulatorInfo->simNodeInfo[iSubNodeNum].cInIP=query.value(4).toString();
                pSimulatorInfo->simNodeInfo[iSubNodeNum].iSimCode = query.value(5).toInt();
                pSimulatorInfo->simNodeInfo[iSubNodeNum].cPCType = query.value(6).toString();
                ++pSimulatorInfo->iSubNodeNum;
            }
       }
       query.clear();
   }
   else
   {
       QMessageBox msgBox;
       msgBox.setText(tr("模拟器节点计算机信息表 打开失败！"));
       msgBox.exec();
   }
}

void DBMaintainToolDlg::WriteToSimulatorCodeTable()
{
    QSqlQuery query;
    QString strSQL=QString("DELETE FROM %1").arg(theDB.m_strSimulatorCodeTable);
    query.prepare(strSQL);
    query.exec();
    strSQL.clear();
    strSQL=QString("INSERT INTO %1 (NAME, SIM_CODE, LOCATION, SIM_GROUP) VALUES (?,?,?,?)").arg(theDB.m_strSimulatorCodeTable);

    for (int i=0; i<m_vSimulatorInfo.size();++i)
    {
        query.prepare(strSQL);
        query.bindValue(0, m_vSimulatorInfo.at(i)->cName);
        query.bindValue(1, m_vSimulatorInfo.at(i)->iSimCode);
        query.bindValue(2, m_vSimulatorInfo.at(i)->cLocation);
        query.bindValue(3, m_vSimulatorInfo.at(i)->cGroup);
        query.exec();
    }
}

void DBMaintainToolDlg::WriteToSimNodePCInfoTable()
{
    QSqlQuery query;
    QString strSQL=QString("DELETE FROM %1").arg(theDB.m_strSimNodePCInfoTable);
    query.prepare(strSQL);
    query.exec();
    int index=0;
    strSQL.clear();
    strSQL=QString("INSERT INTO %1 (ORDER_NUM, NAME, MAC, IP, IN_FACE_IP,RABBITMQ_IP, SIM_CODE,PC_TYPE) VALUES (?,?,?,?,?,?,?,?)").arg(theDB.m_strSimNodePCInfoTable);

    for (int i=0; i<m_vSimulatorInfo.size();++i)
    {
        for(int j=0;j<m_vSimulatorInfo.at(i)->iSubNodeNum;++j)
        {
            ++index;
            query.prepare(strSQL);
            query.bindValue(0,index);
            query.bindValue(1,m_vSimulatorInfo.at(i)->simNodeInfo[j].cName);
            query.bindValue(2,m_vSimulatorInfo.at(i)->simNodeInfo[j].cMac);
            query.bindValue(3,m_vSimulatorInfo.at(i)->simNodeInfo[j].cIP);
            query.bindValue(4,m_vSimulatorInfo.at(i)->simNodeInfo[j].cInIP);
            query.bindValue(5,m_vSimulatorInfo.at(i)->simNodeInfo[j].cServerIP);
            query.bindValue(6,m_vSimulatorInfo.at(i)->simNodeInfo[j].iSimCode);
            query.bindValue(7,m_vSimulatorInfo.at(i)->simNodeInfo[j].cPCType);
            query.exec();
        }
    }
}

void DBMaintainToolDlg::WriteToSimulatorCodeTableTxt()
{
    QString strCfgPath=theBG.GetAppCurPath();
    strCfgPath.append(tr("/模拟器编码表.txt"));
    QFile file(strCfgPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("模拟器编码表.txt 文件打开失败！"));
        msgBox.exec();
        return;
    }

    QTextStream outSimCode(&file);
    outSimCode<<tr("//模拟器名称 模拟器编码 模拟器类型 模拟器位置 模拟器编组\n");
    for (int i=0; i<m_vSimulatorInfo.size();++i)
    {
        outSimCode<<m_vSimulatorInfo.at(i)->cName.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->iSimCode<<"\t"\
                <<m_vSimulatorInfo.at(i)->cLocation.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->cGroup.trimmed()<<"\n";
    }
}

void DBMaintainToolDlg::WriteToSimNodePCInfoTableTxt()
{
    QString strCfgPath=theBG.GetAppCurPath();
    strCfgPath.append(tr("/模拟器节点计算机信息表.txt"));
    QFile fileSimNodeInfo(strCfgPath);
    if (!fileSimNodeInfo.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QMessageBox msgBox;
        msgBox.setText(tr("模拟器节点计算机信息表.txt 文件打开失败！"));
        msgBox.exec();
        return;
    }

    QTextStream out(&fileSimNodeInfo);
    out<<tr("//计算机名称 MAC IP 接口机IP 模拟器编码 计算机类型\n");
    for (int i=0; i<m_vSimulatorInfo.size();++i)
    {
        for(int j=0;j<m_vSimulatorInfo.at(i)->iSubNodeNum;++j)
        {
            out<<m_vSimulatorInfo.at(i)->simNodeInfo[j].cName.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->simNodeInfo[j].cMac.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->simNodeInfo[j].cIP.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->simNodeInfo[j].cInIP.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->simNodeInfo[j].cServerIP.trimmed()<<"\t"\
                <<m_vSimulatorInfo.at(i)->simNodeInfo[j].iSimCode<<"\t"\
                <<m_vSimulatorInfo.at(i)->simNodeInfo[j].cPCType.trimmed()<<"\n";
        }
    }
}

SimulatorInfo* DBMaintainToolDlg::FindSimulatorInfo(int iSimCode)
{

    for (int i = 0; i < m_vSimulatorInfo.size(); ++i)
    {
        if(iSimCode==m_vSimulatorInfo.at(i)->iSimCode)
            return m_vSimulatorInfo.at(i);
    }

    return NULL;
}

void DBMaintainToolDlg::CBSimCode()
{
    m_bSimCode=!m_bSimCode;
}

void DBMaintainToolDlg::CBSimNode()
{
    m_bSimNode=!m_bSimNode;
}

void DBMaintainToolDlg::BtnOutput()
{
    ReadConfigFromDB();
    if(m_bSimCode&&(!m_bSimNode))
    {
        WriteToSimulatorCodeTableTxt();
    }
    if((!m_bSimCode)&&m_bSimNode)
    {
        WriteToSimNodePCInfoTableTxt();
    }
    if(m_bSimCode&&m_bSimNode)
    {
        WriteToSimulatorCodeTableTxt();
        WriteToSimNodePCInfoTableTxt();
    }
    QMessageBox msgBox;
    msgBox.setText(tr("数据导出 完成！"));
    msgBox.exec();
}

void DBMaintainToolDlg::BtnInput()
{
    ReadConfigFromTxt();
    if(m_bSimCode&&(!m_bSimNode))
    {
        WriteToSimulatorCodeTable();
    }
    if((!m_bSimCode)&&m_bSimNode)
    {
        WriteToSimNodePCInfoTable();
    }
    if(m_bSimCode&&m_bSimNode)
    {
        WriteToSimulatorCodeTable();
        WriteToSimNodePCInfoTable();
    }
    QMessageBox msgBox;
    msgBox.setText(tr("数据导入 完成！"));
    msgBox.exec();
}
