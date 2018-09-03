#ifndef CCOMMCFG_H
#define CCOMMCFG_H

#include <QString>
#include "DataStruct.h"

class CCommCfg
{
public:
    CCommCfg();
    void parseCommCfgFile(QString strCfgPath);

public:
    CommType commType;
    QString strCommType;
    QString strCommPara;
    QString strCommOther;
};

#endif // CCOMMCFG_H
