#ifndef SQLite_H
#define SQLite_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QInternal>
#include <QString>
#include <QMap>
#include <QDebug>


class SQLite
{
public:
    SQLite(QString strDBFile,QString strHostName,QString strUserName,QString strPassword);
    ~SQLite();
    /**
     * @brief createTable
     * @param strTableName
     * @param tableAttrMap
     * @return
     */
    bool createTable(QString strTableName,QMap<QString,QString> tableAttrMap);

    /**
     * @brief dataQuery 发送数据库语句
     * @param strQuerySql
     * @return
     */
    int dataQuery(QString strQuerySql);

    /**
     * @brief add 增加数据
     * @param strTableName
     * @param data
     * @return
     */
    int add(QString strTableName,QMap<QString,QString> addData);

    /**
     * @brief del 删除一条记录
     * @param strTableName
     * @param delData
     * @return
     */
    int del(QString strTableName,QMap<QString,QString> where);

    /**
     * @brief updata 更新数据
     * @param strTableName
     * @param where
     * @param data
     * @return
     */
    int updata(QString strTableName,QMap<QString,QString> where,QMap<QString,QString> data);

    /**
     * @brief find 查找
     * @param strTableName
     * @param key
     * @param where
     * @param row
     * @return
     */
    int find(QString strTableName,QList<QString> key,QMap<QString,QString> where,QList<QList<QString>> *row);

    /**
     * @brief find 查找所有
     * @param strTableName
     * @param key
     * @param row
     * @return
     */
    int findAll(QString strTableName,QList<QString> key,QList<QList<QString>> *row);

    /**
     * @brief getSqlQuery
     * @return
     */
    QSqlQuery* getSqlQuery();

private:
    QString getError();

    QSqlDatabase m_sqlDB;
    QSqlQuery *m_pSqlQuery;
    QString m_strDBFile;   //数据库文件名
    QString m_strHostName;  //主机名
    QString m_strUserName;  //用户名
    QString m_strPassword;    //密码

    QSqlError m_sqlError;

};

#endif // SQLite_H
