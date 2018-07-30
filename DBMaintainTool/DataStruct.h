#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QString>

struct SimNodeInfo
{
    QString cName;
    QString cMac;
    QString cIP;
    QString cInIP;
    QString cServerIP;
    int     iSimCode;
    QString cPCType;
};

struct SimulatorInfo
{
    QString cName;                  //模拟器名称
    int     iSimCode;               //模拟器编码
    QString cLocation;              //模拟器位置
    QString cGroup;                 //编组
    int  iSubNodeNum;				//模拟器计算机个数
    SimNodeInfo simNodeInfo[32];	//计算机节点信息数组
};




#endif // DATASTRUCT_H
