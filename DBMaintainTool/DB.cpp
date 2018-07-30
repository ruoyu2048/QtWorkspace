#include "DB.h"
#include "GlobalDef.h"
#include "QSqlQuery"
#include <QTextCodec>


CDB theDB;

CDB::CDB()
{

}

void CDB::GetDatabaseInfo()
{
    m_strDatabase=theBG.GetParameterFromConfig("SQL","DATABASE");
    m_strServerIP=theBG.GetParameterFromConfig("SQL","SERVER");
    m_strUserName=theBG.GetParameterFromConfig("SQL","USER");
    m_strPassword=theBG.GetParameterFromConfig("SQL","PWD");
    m_strExePath=theBG.GetAppCurPath();;
}

void CDB::GetDatasheetName()
{
    m_strSimulatorCodeTable=theBG.GetParameterFromConfig("DS","SHEET1");
    m_strSimNodePCInfoTable=theBG.GetParameterFromConfig("DS","SHEET2");
}

bool CDB::ConnectDatabase()
{
    m_QSqlDB = QSqlDatabase::addDatabase("QODBC");
    QString strDSN=QString("DRIVER={SQL SERVER};\
                           SERVER=%1;DATABASE=%2;\
                           UID=%3;PWD=%4").arg(m_strServerIP).arg(m_strDatabase).arg(m_strUserName).arg(m_strPassword);

    m_QSqlDB.setDatabaseName(strDSN);

    if(m_QSqlDB.open())
    {
        return true;
    }
    return false;
}

void CDB::CloseDatabase()
{
    m_QSqlDB.commit();
    m_QSqlDB.close();
}

void CDB::GetSimulatorInfo()
{
//   m_iAllSimulatorNum = 0;
//   m_strSimulatorCodeTable = theBG.GetParameterFromConfig("DS","SHEET1");
//   QSqlQuery query(m_QSqlDB);
//   QString strQuery = QString("SELECT * FROM %1").arg(m_strSimulatorCodeTable);
//   query.prepare(strQuery);
//   if(query.exec())
//   {
//       while(query.next())
//       {
////           m_SimulatorInfo[m_iAllSimulatorNum].cName=query.value(0).toString();
////           m_SimulatorInfo[m_iAllSimulatorNum].iSimID = (m_iAllSimulatorNum+1)*100;
////           m_SimulatorInfo[m_iAllSimulatorNum].iSimCode=query.value(1).toInt();
////           m_SimulatorInfo[m_iAllSimulatorNum].cSimType=query.value(2).toString();
////           m_SimulatorInfo[m_iAllSimulatorNum].cLocation=query.value(3).toString();
////           m_SimulatorInfo[m_iAllSimulatorNum].cGroup=query.value(4).toString();
//           ++m_iAllSimulatorNum;
//       }
//       query.clear();
//   }
}

void CDB::GetSimNodePCInfo()
{
//   m_strSimNodePCInfoTable = theBG.GetParameterFromConfig("DS","SHEET2");
//   QSqlQuery query(m_QSqlDB);
//   QString strQuery = QString("SELECT * FROM %1").arg(m_strSimNodePCInfoTable);
//   query.prepare(strQuery);
//   if(query.exec())
//   {
//       while(query.next())
//       {
//            SimulatorInfo* pSimulatorInfo=FindSimulatorInfo(query.value(5).toInt());
//            if(NULL != pSimulatorInfo)
//            {
////                int iSubNodeNum=pSimulatorInfo->iSubNodeNum;
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].iIndex = query.value(0).toInt();
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].cName=query.value(1).toString();
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].iSimNodeID = pSimulatorInfo->iSimID+iSubNodeNum+1;
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].cMac=query.value(2).toString();
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].cIP=query.value(3).toString();
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].cInIP=query.value(4).toString();
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].iSimCode = query.value(5).toInt();
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].iPCType = query.value(6).toInt();
////                //Software state
////                pSimulatorInfo->simNodeInfo[iSubNodeNum].iSWNum = 0;
////                ++pSimulatorInfo->iSubNodeNum;
//            }
//       }
//       query.clear();
//   }
//   InitSimulatorInfoMap();
}
