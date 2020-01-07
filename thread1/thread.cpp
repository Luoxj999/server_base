#include "thread.h"
#include "threadpool.h"

Thread::Thread(ThreadPool* threadPool):
    task_(NULL),
    threadPool_(threadPool)
{
    pthread_cond_init(&cond_, NULL);
    pthread_mutex_init(&mutex_, NULL);
}

Thread::~Thread() {
    pthread_mutex_destroy(&mutex_);
    pthread_cond_destroy(&cond_);
}

void* Thread::threadFunc(void* arg)
{
    pthread_detach(pthread_self());

    Thread* t = static_cast<Thread*>(arg);

    std::cout << "thread <" << t->tid() << "> " << pthread_self() << std::endl; 

    ThreadPool* pool = t->threadPool();

    bool isRun = true;
    while (isRun) 
    {
        if (t->threadPool_->finished()) {
            std::cout << t->tid() << " finished." << std::endl;
            break;
        }

        if (t->getTask() == NULL) {
            Task* task = pool->popTask();
            if (task == NULL) 
            {
                t->waitCond();
                std::cout << "thread wait out " << t->tid() << std::endl;
                continue;
            }
            t->setTask(task);
        } 
        // std::cout << "run enter " << t->tid() << std::endl;
        t->process();
        t->afterProcess();
        if (t->threadPool_->finished()) {
            std::cout << t->tid() << " finished." << std::endl;
            break;
        }
    }

    std::cout << "thread finish all." << std::endl;
}

pthread_t Thread::createThread()
{
    if (pthread_create(&tid_, NULL, Thread::threadFunc, (void*)this) != 0)
    {
        LOG_ERROR("createThread error.");
        return 0;
    }
    return tid_;
}

void Thread::waitCond()
{
    pthread_mutex_lock(&mutex_);
    pthread_cond_wait(&cond_, &mutex_);
    pthread_mutex_unlock(&mutex_);
}

void Thread::signalCond()
{
    pthread_mutex_lock(&mutex_);
    pthread_cond_signal(&cond_);
    pthread_mutex_unlock(&mutex_);
}

void Thread::afterProcess() 
{
    delete task_;
    task_ = NULL;
    
    threadPool_->freeThread(this);
}
