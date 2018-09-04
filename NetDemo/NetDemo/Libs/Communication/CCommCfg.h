#ifndef CCOMMCFG_H
#define CCOMMCFG_H

#include <QString>
#include "DataStruct.h"
/*------------------------------------------------------------*/
/**********************配置文件格式说明与参考**********************/
/*-------------------------------------------------------------|
 --------------------------------------------------------------|
 | (1)TcpServer                                                |
 | {                                                           |
 |     "comm_type":"TcpServer",//标识符必须与此一致               |
 |     "comm_para":"127.0.0.1:9999",//ServerIP:Port            |
 |     "comm_other":""                                         |
 | }                                                           |
 |-------------------------------------------------------------|
 |(2)TcpClient                                                 |
 | {                                                           |
 |     "comm_type":"TcpClient",//标识符必须与此一致               |
 |     "comm_para":"127.0.0.1:9999",                           |
 |     "comm_other":"0x30,0x40"                                |
 | }                                                           |
 |-------------------------------------------------------------|
 |(3)UDP                                                       |
 | {                                                           |
 |     "comm_type":"UDP",//标识符必须与此一致                     |
 |     "comm_para":"127.0.0.1:9999",                           |
 |     "comm_other":""                                         |
 | }                                                           |
 |-------------------------------------------------------------|
 | (4)Serial                                                   |
 | {                                                           |
 |     "comm_type":"Serial",//标识符必须与此一致                  |
 |     "comm_para":"9600,8,0,1,0",                             |
 |     "comm_other":""                                         |
 | }                                                           |
 |-------------------------------------------------------------|
 |------------------------------------------------------------*/

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
