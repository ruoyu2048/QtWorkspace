#ifndef DB_H
#define DB_H
#include <QString>
#include <QVariant>
#include <QMap>
#include <QVector>
#include <QtSql/QSqlDatabase>

class CDB
{
public:
    CDB();
    ~CDB();
    static QString GetDBServer();
    static QString GetDBName();
    static QString GetDBUserName();
    static QString GetDBPWD();
    static QString GetParameterFromConfig(QString strSecion,QString strKey);
    static void SetParameterFromConfig(QString strSecion,QString strKey, QString strValue);

    static bool ConnectDatabase();
    static QString GetNameByIP(QString strIP);
    static QString GetNameByIP(QString strIP, QString strIf);
    static void CloseDatabase();

    static const char* UnicodeCharToChar(QVariant var);

public:
    static QSqlDatabase m_QSqlDB;
};

#endif // DB_H
