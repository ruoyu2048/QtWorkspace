#include "Thread01.h"
#include <QDebug>

Thread01::Thread01()
{

}

void Thread01::run()
{
    qDebug()<<"Thread01_ID:"<<this->currentThreadId();
}
