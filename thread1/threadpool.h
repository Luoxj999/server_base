#pragma once

#include <pthread.h>
#include <queue>
#include <list>
#include <iostream>

#include "task.h"
#include "common.h"

class Thread;
class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    Thread* createThread();
    bool createThreadPool(int maxThreadCount);
    Task* popTask();
    void addTask(Task* task);
    void freeThread(Thread* thread);
    void pushTask(Task* task);

    void finish(bool finished) 
    {
        // pthread_mutex_lock(&bufferedTaskListMutex_);
        isFinished = finished;
        // pthread_mutex_unlock(&bufferedTaskListMutex_);
    }

    bool finished()
    {
        // bool ret = false;
        // pthread_mutex_lock(&bufferedTaskListMutex_);
        // if (isFinished && bufferedTaskList_.empty()) 
        // {
            // ret = true;
        // }
        // pthread_mutex_unlock(&bufferedTaskListMutex_);
        // return ret;
        return isFinished;
    }

    void destroy();

protected:
    bool isInitialize_;
    std::queue<Task*> bufferedTaskList_; // 系统繁忙时还未处理的任务列表
    
    pthread_mutex_t bufferedTaskListMutex_;
 
    std::list<Thread*> busyThreadList_;
    std::list<Thread*> freeThreadList_;
    std::list<Thread*> allThreadList_;
    int currThreadCount_;
    int currFreeThreadCount_;
    int maxThreadCount_;

    pthread_mutex_t threadListMutex_;

    bool isFinished;
};