#ifndef DB_H
#define DB_H

#include <QList>
#include <QtSql/QSqlDatabase>
class DB
{

public:
    DB();
    bool OpenDB(QString strDBName);
    bool CreateTable(QString strTableName,QString &strSQL);
    bool DeleteTable(QString strTableName);
    bool Insert(QString &strSQL);
    bool Update(QString &strSQL);
    bool Delete(QString &strSQL);
    bool Select(QString &strSQL,QList<QStringList> &tableValues);
    void CloseDB();

private:
    QString         m_strDBName;//数据库名称
    QString         m_strTBName;//记录当前要操作的数据库表名
    QSqlDatabase    m_QSqlDB;    //数据库对象
};

#endif // DB_H
