#ifndef CHOTUPDATEENUMDEF_H
#define CHOTUPDATEENUMDEF_H

//发送文件类型
enum SendType{
    File,       //以单个文件发送
    Dir,        //以整个文件夹发送
    Configure   //以配置文件中的信息发送
};

//当前文件的传输状态
enum TransferState{
    Start,      //传输开始
    Continue,   //传输继续
    Stop        //传输结束
};

enum TransferResult{
    Success,
    Fail
};

#endif // CHOTUPDATEENUMDEF_H
