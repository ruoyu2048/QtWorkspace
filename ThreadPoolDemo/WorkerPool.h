#ifndef WORKERPOOL_H
#define WORKERPOOL_H

#include <iostream>
#include "IWorkerPool.h"

using namespace std;

//为了多耗点cpu，计算斐波那契数列吧
static int fibonacci(int a)
{
    //ASSERT(a > 0);
    if (a == 1 || a == 2)
        return 1;
    return fibonacci(a-1) + fibonacci(a-2);
}

//异步计算任务
struct AsyncCalcQuest
{
    AsyncCalcQuest():num(0),result(0)
    {}
    //计算需要用到的变量
    int num;
    int result;
};

//为了测试方便,引入全局变量用于标识线程池已将所有计算完成
const int TOTAL_COUNT = 1000000;
static int now_count = 0;

//继承一下线程池类,在子类处理计算完成的业务，在我们这里，只是打印一下计算结果
class WorkerPool:public IWorkerPool<AsyncCalcQuest>
{
public:
    WorkerPool();
    virtual ~WorkerPool();
    //在工人线程中执行
    void DoWork(AsyncCalcQuest *quest);
    //在主线程执行
    void DoCallback();
    void test();

private:
    mutex m_callbackMutex;
    //这里是准备给回调的任务列表
    vector<AsyncCalcQuest*> m_vTaskList;
};

#endif // WORKERPOOL_H
