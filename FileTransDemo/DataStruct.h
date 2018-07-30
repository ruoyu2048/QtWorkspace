#ifndef DATASTRUCT_H
#define DATASTRUCT_H

//文件信息包头
struct FILEINFO_PH
{
    char cFileName[64];         //文件名
    char cDstPath[256];         //目的路径
    int  nFileSize;             //文件大小
    char cTimeCreation[20];     //文件创建时间
    char cTimeLastRead[20];     //文件最后访问时间
    char cTimeLastModify[20];   //文件最后修改时间
};

#endif // DATASTRUCT_H
