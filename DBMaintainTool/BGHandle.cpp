#include "BGHandle.h"
#include <QSettings>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include "GlobalDef.h"

CBGHandle theBG;

CBGHandle::CBGHandle()
{
}

QString CBGHandle::GetAppCurPath()
{
    return QApplication::applicationDirPath();
}

QString CBGHandle::GetParameterFromConfig(QString strSecion,QString strKey)
{
    QString strConfigFilePath=QString("%1/config/config.ini").arg(GetAppCurPath());
    QSettings* readIni=new QSettings(strConfigFilePath,QSettings::IniFormat);
    QString strSK=QString("/%1/%2").arg(strSecion).arg(strKey);
    return readIni->value(strSK).toString();
}

void CBGHandle::PrintLogs(QString strLog)
{
    QString strLogFilePath=QString("%1/logs/zhgk.txt").arg(GetAppCurPath());
    QFile file(strLogFilePath);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        QTextStream writeLog(&file);
        writeLog<<strLog<<endl;
    }
    file.close();
}


