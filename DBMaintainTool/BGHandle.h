#ifndef BGHANDLE_H
#define BGHANDLE_H
#include <QString>
#include <QApplication>

#include "DataStruct.h"


class CBGHandle
{
public:
    CBGHandle();
    QString GetAppCurPath();
    QString GetParameterFromConfig(QString strSecion,QString strKey);
    void PrintLogs(QString strLog);
};

#endif // BGHANDLE_H
