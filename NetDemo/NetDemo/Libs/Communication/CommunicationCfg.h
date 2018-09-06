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
 |     "simm_data":"false",//是否为模拟器                        |
 |     "comm_type":"TcpServer",//标识符必须与此一致               |
 |     "comm_para":"127.0.0.1:9999",//ServerIP:Port            |
 |     "comm_other":""                                         |
 | }                                                           |
 |-------------------------------------------------------------|
 |(2)TcpClient                                                 |
 | {                                                           |
 |     "simm_data":"false",//是否为模拟器                        |
 |     "comm_type":"TcpClient",//标识符必须与此一致               |
 |     "comm_para":"127.0.0.1:9999",                           |
 |     "comm_other":"0x30,0x40"                                |
 | }                                                           |
 |-------------------------------------------------------------|
 |(3)UDP                                                       |
 | {                                                           |
 |     "simm_data":"false",//是否为模拟器                        |
 |     "comm_type":"UDP",//标识符必须与此一致                     |
 |     "comm_para":"127.0.0.1:9999",                           |
 |     "comm_other":""                                         |
 | }                                                           |
 |-------------------------------------------------------------|
 | (4)Serial                                                   |
 | {                                                           |
 |     "simm_data":"false",//是否为模拟器                        |
 |     "comm_type":"Serial",//标识符必须与此一致                  |
 |     "comm_para":"9600,8,0,1,0",                             |
 |     "comm_other":""                                         |
 | }                                                           |
 |-------------------------------------------------------------|
 |------------------------------------------------------------*/

class CommunicationCfg
{
public:
    CommunicationCfg();
    void parseCommCfgFile(QString strCfgPath);

public:
    bool     bSimmulator;   //是否为模拟器
    CommType commType;      //通信类型
    QString  strCommPara;   //启动参数，如IP,Port等
    QString  strCommOther;  //附加参数，如订阅目的地址等
private:
    QString  strSimmulator;
    QString  strCommType;
};

#endif // CCOMMCFG_H
