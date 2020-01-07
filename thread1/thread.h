#pragma once

#include <pthread.h>
#include <queue>
#include <list>
#include <iostream>

#include "task.h"
#include "common.h"

class ThreadPool;
class Thread
{
public:
    friend class ThreadPool;
    Thread(ThreadPool* threadPool);

    ~Thread();
    static void* threadFunc(void* arg);

    pthread_t createThread();

    pthread_t tid()
    {
        return tid_;
    }

    void lock() { pthread_mutex_lock(&mutex_); }
    void unlock() { pthread_mutex_unlock(&mutex_); }

    ThreadPool* threadPool() { return threadPool_; }

    void waitCond();

    void signalCond();

    void process() { task_->process(); }
    void afterProcess();

    void setTask(Task* task) { task_ = task; }
    Task* getTask() { return task_; } 
protected:
    Task* task_;
    pthread_cond_t cond_;
    pthread_mutex_t mutex_;
    pthread_t tid_;
    ThreadPool* threadPool_;
};
