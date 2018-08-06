#ifndef IWORKERPOOL_H
#define IWORKERPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

using namespace std;

template<typename T>
class IWorkerPool
{
public:
    typedef IWorkerPool<T> THIS_TYPE;
    typedef function<void(T*)> WorkerProc;
    typedef vector<thread*> ThreadVec;

//    IWorkerPool();
    IWorkerPool()
    {
        m_bActive = false;
    }

    ~IWorkerPool()
    {
        for(ThreadVec::iterator it = m_vAllThread.begin();it != m_vAllThread.end();++it)
            delete *it;
        m_vAllThread.clear();
    }

    void Start(WorkerProc f,int worker_num)
    {
        m_bActive = true;
        m_vAllThread.resize(worker_num);
        for (int i = 0; i < worker_num;i++ )
        {
            m_vAllThread[i] = new thread(bind(&THIS_TYPE::consumer,this));
        }
        m_workerProc = f;
    }

    //生产者
    void Push(T* pTask)
    {
        unique_lock < mutex > lck(m_taskMutex);
        m_taskQueue.push(pTask);
        m_cv.notify_one();
    }

    void Stop()
    {
        //等待所有的任务执行完毕
        m_taskMutex.lock();
        while (!m_taskQueue.empty())
        {
            m_taskMutex.unlock();
            this_thread::sleep_for(chrono::milliseconds(1000));
            m_cv.notify_one();
            m_taskMutex.lock();
        }
        m_taskMutex.unlock();

        //关闭连接后，等待线程自动退出
        m_bActive = false;
        m_cv.notify_all();
        for(ThreadVec::iterator it = m_vAllThread.begin();
            it != m_vAllThread.end();++it)
            (*it)->join();
    }
    //private:
    //消费者
    void consumer()
    {
        //第一次上锁
        unique_lock<mutex> lck(m_taskMutex);
        while (m_bActive)
        {
            //如果是活动的，并且任务为空则一直等待
            while (m_bActive && m_taskQueue.empty())
                m_cv.wait(lck);

            //如果已经停止则退出
            if(!m_bActive)
                break;

            T* pTask = m_taskQueue.front();
            m_taskQueue.pop();

            //从任务队列取出后该解锁(任务队列锁)了
            lck.unlock();

            //执行任务后释放
            m_workerProc(pTask);

            //delete quest;   //在m_workerProc已经释放该指针了
            //重新上锁
            lck.lock();
        }
    }


    bool                m_bActive;      //线程池活动状态
    mutex               m_taskMutex;    //任务互斥锁
    vector<thread*>     m_vAllThread;   //线程队列
    queue<T*>           m_taskQueue;    //任务队列
    condition_variable  m_cv;           //条件变量
    WorkerProc          m_workerProc;   //处理单元
};

#endif // IWORKERPOOL_H
