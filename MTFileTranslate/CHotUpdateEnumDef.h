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
    Start,
    Continue,
    Stop
};

#endif // CHOTUPDATEENUMDEF_H
