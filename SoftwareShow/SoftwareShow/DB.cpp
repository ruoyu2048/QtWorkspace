#include "DB.h"
#include "QSqlQuery"
#include <QTextCodec>
#include <QDebug>
#include <QSettings>
#include <QApplication>

QSqlDatabase CDB::m_QSqlDB;

CDB::CDB()
{

}

CDB::~CDB()
{

}

QString CDB::GetDBServer()
{
    return GetParameterFromConfig("SQL","SERVER");
}

QString CDB::GetDBName()
{
    return GetParameterFromConfig("SQL","DATABASE");
}
QString CDB::GetDBUserName()
{
    return GetParameterFromConfig("SQL","USER");
}
QString CDB::GetDBPWD()
{
    return GetParameterFromConfig("SQL","PWD");
}

QString CDB::GetParameterFromConfig(QString strSecion,QString strKey)
{
    QString strConfigFilePath=QString("%1/config/config.ini").arg(QApplication::applicationDirPath());
    QSettings* readIni=new QSettings(strConfigFilePath,QSettings::IniFormat);
    QString strSK=QString("/%1/%2").arg(strSecion).arg(strKey);
    return readIni->value(strSK).toString();
}

void CDB::SetParameterFromConfig(QString strSecion,QString strKey, QString strValue)
{
    QString strConfigFilePath=QString("%1/config/config.ini").arg(QApplication::applicationDirPath());
    QSettings* writeIni=new QSettings(strConfigFilePath,QSettings::IniFormat);
    QString strSK=QString("/%1/%2").arg(strSecion).arg(strKey);
    writeIni->setValue(strSK, strValue);
}

QString CDB::GetNameByIP(QString strIP)
{
    QString strName;
    QSqlQuery query(m_QSqlDB);
    QString strQuery = QString("SELECT * FROM SimNodePCManage_207 WHERE IP地址 = \'%1\'").arg(strIP);
    query.prepare(strQuery);
    if(query.exec())
    {
        if(query.next())
        {
            strName = query.value(0).toString();
        }
        query.clear();
    }
    return strName;
}

QString CDB::GetNameByIP(QString strIP, QString strIf)
{
    QString strName;
    QSqlQuery query(m_QSqlDB);
    QString strQuery = QString("SELECT * FROM SimNodePCManage_207 WHERE ((IP地址 = \'%1\') AND (备注 = \'%2\'))").arg(strIP).arg(strIf);
    query.prepare(strQuery);
    if(query.exec())
    {
        if(query.next())
        {
            strName = query.value(0).toString();
        }
        query.clear();
    }
    return strName;
}

bool CDB::ConnectDatabase()
{
    m_QSqlDB = QSqlDatabase::addDatabase("QODBC");
    QString strDSN=QString("DRIVER={SQL SERVER};\
                           SERVER=%1;DATABASE=%2;\
                           UID=%3;PWD=%4").arg(CDB::GetDBServer()).arg(CDB::GetDBName()).arg(CDB::GetDBUserName()).arg(CDB::GetDBPWD());

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

const char* CDB::UnicodeCharToChar(QVariant var)
{
    QString str=QString::fromUtf8(var.toByteArray());
    return str.toStdString().c_str();
}
