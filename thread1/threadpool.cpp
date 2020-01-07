#include "thread.h"
#include "threadpool.h"
#include <pthread.h>

ThreadPool::ThreadPool():
    bufferedTaskList_(),
    busyThreadList_(),
    freeThreadList_(),
    allThreadList_(),
    currThreadCount_(0),
    currFreeThreadCount_(0),
    maxThreadCount_(0),
    isFinished(false)
{
    pthread_mutex_init(&bufferedTaskListMutex_, NULL);
    pthread_mutex_init(&threadListMutex_, NULL);
}

ThreadPool::~ThreadPool()
{
    // for (int i = 0; i < maxThreadCount_; i++)
    // {
    //     Thread *t = allThreadList_.front();
    //     allThreadList_.pop_back();
    //     t->signalCond();
    //     delete t;
    // }

    pthread_mutex_destroy(&threadListMutex_);
    pthread_mutex_destroy(&bufferedTaskListMutex_);
}

Thread* ThreadPool::createThread()
{
    Thread* thread = new Thread(this);
    thread->createThread();

    return thread;
}

bool ThreadPool::createThreadPool(int maxThreadCount)
{
    if (isInitialize_) 
    {
        LOG_ERROR("threadPool is initialized.");
        return false;
    }

    maxThreadCount_ = maxThreadCount;
    for (int i = 0; i < maxThreadCount_; i++)
    {
        Thread* t = createThread();

        currThreadCount_++;
        currFreeThreadCount_++;

        allThreadList_.push_back(t);
        freeThreadList_.push_back(t);
    }

    isInitialize_ = true;
    return true;
}

Task* ThreadPool::popTask()
{
    pthread_mutex_lock(&bufferedTaskListMutex_);

    if (bufferedTaskList_.empty())
    {
        pthread_mutex_unlock(&bufferedTaskListMutex_);
        return NULL;
    }

    Task* ret = bufferedTaskList_.front();
    bufferedTaskList_.pop();

    pthread_mutex_unlock(&bufferedTaskListMutex_);

    return ret;
}

void ThreadPool::addTask(Task* task)
{
    Thread* t = NULL;
    pthread_mutex_lock(&threadListMutex_);
    std::cout << "addTask freeThreadList empty? " << freeThreadList_.empty() << " " << freeThreadList_.size() << std::endl;
    if (!freeThreadList_.empty()) {
        t = freeThreadList_.back();
        freeThreadList_.pop_back();
        busyThreadList_.push_back(t);
        std::cout << "signal" << std::endl;
    }
    pthread_mutex_unlock(&threadListMutex_);

    if (t != NULL) {
        t->setTask(task);
        t->signalCond();
    }
    else
    {
        pushTask(task);
    }
}

void ThreadPool::freeThread(Thread* thread)
{
    pthread_mutex_lock(&threadListMutex_);
    for (auto it = busyThreadList_.begin(); it != busyThreadList_.end(); it++)
    {
        if ((*it)->tid() == thread->tid()) {
            busyThreadList_.erase(it);
            break;
        }
    }

    freeThreadList_.push_back(thread);
    pthread_mutex_unlock(&threadListMutex_);
}

void ThreadPool::pushTask(Task* task)
{
    pthread_mutex_lock(&bufferedTaskListMutex_);

    bufferedTaskList_.push(task);

    pthread_mutex_unlock(&bufferedTaskListMutex_);
}

void ThreadPool::destroy()
{
    finish(true);

    pthread_mutex_lock(&bufferedTaskListMutex_);
    while (bufferedTaskList_.size() > 0)
    {
        Task* task = bufferedTaskList_.front();
        bufferedTaskList_.pop();
        delete task;
    }
    pthread_mutex_unlock(&bufferedTaskListMutex_);

    pthread_mutex_lock(&threadListMutex_);
    for (auto it = freeThreadList_.begin(); it != freeThreadList_.end(); it++)
    {
        (*it)->signalCond();
    }

    for (auto it = allThreadList_.begin(); it != allThreadList_.end();)
    {
        delete (*it);
        (*it) = NULL;
        it = allThreadList_.erase(it);
    }
    pthread_mutex_unlock(&threadListMutex_);
}