#include "WorkerPool.h"

WorkerPool::WorkerPool(){

}

WorkerPool::~WorkerPool()
{
}

//在工人线程中执行
void WorkerPool::DoWork(AsyncCalcQuest *quest)
{
    quest->result = fibonacci(quest->num);
    //并将已完成任务返回到准备回调的列表
    unique_lock<mutex > lck(m_callbackMutex);
    m_vTaskList.push_back(quest);
}

//在主线程执行
void WorkerPool::DoCallback()
{
    //组回调任务上锁
    unique_lock<mutex > lck(m_callbackMutex);
    while (!m_vTaskList.empty())
    {
        auto *quest = m_vTaskList.back();
        {//此处为业务代码打印一下吧
            cout << quest->num << " " << quest->result << endl;
            now_count ++;
        }
        delete quest;		//TODO:这里如果采用内存池就更好了
        m_vTaskList.pop_back();
    }
}

void WorkerPool::test(){
    //工厂开工了 8个工人喔
    this->Start(bind(&WorkerPool::DoWork,this,placeholders::_1),8);
    //开始产生任务了
    for (int i=0; i<TOTAL_COUNT; i++)
    {
        AsyncCalcQuest* pTask = new AsyncCalcQuest;
        pTask->num = i%40+1;
        this->Push(pTask);
    }

    //int now_count = 0;
    while (now_count != TOTAL_COUNT)
    {
        this->DoCallback();
    }

    this->Stop();
}
