#include "DB.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

DB::DB(QObject *parent) : QObject(parent)
{

}

bool DB::OpenDB(QString strDBName)
{
    m_strDBName = strDBName;
    if( !m_strDBName.contains(".db") )
        m_strDBName.append(".db");

    m_QSqlDB = QSqlDatabase::addDatabase("QSQLITE");
    m_QSqlDB.setDatabaseName(m_strDBName);
    m_QSqlDB.setHostName("");
    m_QSqlDB.setUserName("");
    m_QSqlDB.setPassword("");
    return m_QSqlDB.open();
}

bool DB::CreateTable(QString strTableName,QString &strSQL)
{
    QSqlQuery sqlQuery(m_QSqlDB);
    QString strTBSql = QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(strTableName);
    if(true == sqlQuery.exec(strTBSql) )
    {
        if( true ==sqlQuery.next() && 1 == sqlQuery.value(0).toInt())
        {
            qDebug()<<"The Table exit!";
            return true;
        }
    }

    if( false == sqlQuery.exec(strSQL) )
    {
        qDebug()<<"CreateTable failed,ErrorInfo:"<<sqlQuery.lastError();
        return false;
    }
    return true;
}

bool DB::DeleteTable(QString strTableName)
{
    QSqlQuery sqlQuery(m_QSqlDB);
    QString strDelTable = QString("drop table %1").arg(strTableName);
    if( false == sqlQuery.exec(strDelTable) )
    {
        qDebug()<<"DeleteTable failed,ErrorInfo:"<<sqlQuery.lastError();
        return false;
    }
    return true;
}

bool DB::Insert(QString &strSQL)
{
    QSqlQuery sqlQuery(m_QSqlDB);
    if( false == sqlQuery.exec(strSQL) )
    {
        qDebug()<<"Insert failed,ErrorInfo:"<<sqlQuery.lastError();
        return false;
    }
    return true;
}

bool DB::Update(QString &strSQL)
{
    QSqlQuery sqlQuery(m_QSqlDB);
    if( false == sqlQuery.exec(strSQL) )
    {
        qDebug()<<"Update failed,ErrorInfo:"<<sqlQuery.lastError();
        return false;
    }
    return true;
}

bool DB::Delete(QString &strSQL)
{
    QSqlQuery sqlQuery(m_QSqlDB);
    if( false == sqlQuery.exec(strSQL) )
    {
        qDebug()<<"Delete failed,ErrorInfo:"<<sqlQuery.lastError();
        return false;
    }
    return true;
}

bool DB::Select(QString &strSQL,QList<QStringList> &tableValues)
{
    QSqlQuery sqlQuery(m_QSqlDB);
    if( false == sqlQuery.exec(strSQL) )
    {
        qDebug()<<"Select failed,ErrorInfo:"<<sqlQuery.lastError();
        return false;
    }

    tableValues.clear();
    while(sqlQuery.next())
    {
        QStringList rowValues;
        int nCol = sqlQuery.size();
        for( int i=0; i<nCol; ++i )
        {
            rowValues<<sqlQuery.value(i).toString();
        }
        tableValues.push_back(rowValues);
    }

    return true;
}

void DB::CloseDB()
{
    m_QSqlDB.close();
}
