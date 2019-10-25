#include "SQLite.h"

SQLite::SQLite(QString strDBFile, QString strHostName, QString strUserName, QString strPassword)
{
    m_strHostName=strHostName;
    m_strUserName=strUserName;
    m_strPassword=strPassword;
    m_strDBFile=strDBFile;
    m_sqlDB=QSqlDatabase::addDatabase("QSQLITE");
    m_pSqlQuery=new QSqlQuery(m_sqlDB);
    m_sqlDB.setHostName(m_strHostName);
    m_sqlDB.setUserName(m_strUserName);
    m_sqlDB.setPassword(m_strPassword);
    m_sqlDB.setDatabaseName(m_strDBFile);
    if(!m_sqlDB.open())
    {
        m_sqlError = m_sqlDB.lastError();
        qDebug()<<"Open database failed,the error is"<<m_sqlDB.lastError().text();
    }
}


SQLite::~SQLite()
{
    m_sqlDB.close();
}

bool SQLite::createTable(QString strTableName, QMap<QString, QString> tableAttrMap)
{
    QSqlQuery query;
    QString sql="create table "+strTableName+" (";
    for(QMap<QString,QString>::const_iterator it=tableAttrMap.constBegin();it!=tableAttrMap.constEnd();it++)
    {
        sql+=it.key()+' '+it.value();
        if(it!=tableAttrMap.constEnd())
            sql+=',';
    }
    sql.chop(1);
    sql+=")";
    qDebug()<<sql;
    query.prepare(sql);
    return query.exec();
}

int SQLite::dataQuery(QString strQuerySql)
{
    m_pSqlQuery->prepare(strQuerySql);
    return m_pSqlQuery->exec();
}

int SQLite::add(QString strTableName, QMap<QString, QString> addData)
{
    QString sql="insert into "+strTableName+  "(";
    QString values=" values(";
    for(QMap<QString,QString>::const_iterator i=addData.constBegin();i!=addData.constEnd();i++)
    {
        sql+=i.key()+", ";
        values+=i.value()+", ";
    }
    sql.chop(2);
    values.chop(2);
    sql+=")";
    values+=")";
    sql+=values;
    qDebug()<<sql;
    m_pSqlQuery->prepare(sql);
    return m_pSqlQuery->exec();
}

int SQLite::del(QString strTableName, QMap<QString, QString> where)
{
    QString sql="delete ";
    sql+=strTableName;
    sql+=" where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"=";
        sql+="'"+i.value()+"' ";
    }
    sql.chop(2);
    m_pSqlQuery->prepare(sql);
    return m_pSqlQuery->exec();
}

int SQLite::updata(QString strTableName, QMap<QString, QString> where, QMap<QString, QString> data)
{
    QString sql="updata "+strTableName+" set";
    for(QMap<QString,QString>::const_iterator i=data.constBegin();i!=data.constEnd();i++)
    {
        sql+=i.key()+"=";
        sql+=i.value()+" ";
    }
    sql+="where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"=";
        sql+=i.value()+" ";
    }
    return m_pSqlQuery->exec();
}

int SQLite::find(QString strTableName, QList<QString> key, QMap<QString, QString> where, QList<QList<QString> > *row)
{
    QString sql="select ";
    int len=key.size();
    for(int i=0;i<len;i++)
    {
        sql+=key.at(i);
        sql+=",";
    }
    sql.chop(1);
    sql+=" from "+strTableName;
    sql+=" where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"="+i.value()+",";
    }
    sql.chop(1);
    //qDebug()<<sql;
    m_pSqlQuery->prepare(sql);
    if(m_pSqlQuery->exec())
    {
        while (m_pSqlQuery->next()) {
            QList<QString> j;
            for(int i=0;i<len;i++)
            {
                j.append(m_pSqlQuery->value(i).toString());
            }
            row->append(j);
        }
        return 1;
    }
    else
        return 0;
}

int SQLite::findAll(QString strTableName, QList<QString> key, QList<QList<QString> > *row)
{
    QString sql="select ";
    int len=key.size();
    for(int i=0;i<len;i++)
    {
        sql+=key.at(i);
        sql+=",";
    }
    sql.chop(1);
    sql+=" from "+strTableName;
    //qDebug()<<sql;
    m_pSqlQuery->prepare(sql);
    if(m_pSqlQuery->exec())
    {
        while (m_pSqlQuery->next()) {
            QList<QString> j;
            for(int i=0;i<len;i++)
            {
                j.append(m_pSqlQuery->value(i).toString());
            }
            row->append(j);
        }
        return 1;
    }
    else return 0;
}

QSqlQuery *SQLite::getSqlQuery()
{
    return m_pSqlQuery;
}

QString SQLite::getError()
{
    return m_sqlError.text();
}
