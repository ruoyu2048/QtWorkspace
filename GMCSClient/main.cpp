#include "GMCSClient.h"
#include <QApplication>
#include <QSystemSemaphore>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //创建共享内存,判断是否已经运行程序
    QSharedMemory mem("GMCSClient");
    if (!mem.create(1))
    {
        QMessageBox::information(NULL,"提示","综合管控客户端程序已运行！",QMessageBox::Ok);
        return 1;
    }
    GMCSClient w;
    w.show();

    return a.exec();
}
