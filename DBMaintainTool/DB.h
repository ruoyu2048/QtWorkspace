#ifndef DB_H
#define DB_H
#include <QString>
#include <QVariant>
#include <QMap>
#include <QtSql/QSqlDatabase>
#include "MacroDef.h"
#include "DataStruct.h"

class CDB
{
public:
    CDB();
    void GetDatabaseInfo();
    void GetDatasheetName();
    bool ConnectDatabase();
    void CloseDatabase();

    void GetSimulatorInfo();
    void GetSimNodePCInfo();

public:
    QString	m_strDatabase;
    QString	m_strServerIP;
    QString m_strUserName;
    QString	m_strPassword;
    QString	m_strExePath;

    QString	m_strSimulatorCodeTable;
    QString	m_strSimNodePCInfoTable;

    QSqlDatabase m_QSqlDB;
};

#endif // DB_H
