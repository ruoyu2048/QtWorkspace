#ifndef CHOTUPDATEDATASTRUCT_H
#define CHOTUPDATEDATASTRUCT_H
#include <QtGlobal>
#include <memory.h>
#include "CHotUpdateEnumDef.h"

//#pragma pack(4)
typedef struct _FileTransferInfo{//文件传输信息
    qint32 nTotal;              //总文件个数
    qint32 nIndex;              //当前文件序号
    qint64 nFileSzie;           //文件大小=sizeof(FileTransferInfo)+文件实际大小
    char cFileName[128];        //文件名称
    char cFileSrcPath[256];     //文件路径
    char cFileDstPath[256];     //文件路径
    char cMD5[64];              //MD5码
    SendType sendType;          //发送类型
    TransferState transferState;//文件发送状态
    TransferResult transferResult;//传输结果
    _FileTransferInfo(){
        memset(&nTotal,0,sizeof (_FileTransferInfo));
    }
}FileTransferInfo;

//文件传输进度信息
typedef struct _FileUpdateInfo{
    int nTotal;             //文件总个数
    int nIndex;             //当前文件序号
    double dProcess;        //传输进度
    char cHandleFlag[32];   //服务端使用的标识参数，客户端不使用
    char cFileName[128];    //文件名
    char cFilePath[256];    //文件所在文件夹绝对路径
    _FileUpdateInfo(){
        memset(&nTotal,0,sizeof (_FileUpdateInfo));
    }
}FileUpdateInfo;
//#pragma pack()

#endif // CHOTUPDATEDATASTRUCT_H
