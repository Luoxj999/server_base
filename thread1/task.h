#pragma once

#include <pthread.h>

class Task
{
public:
    virtual ~Task() {}
    virtual bool process() = 0;
};

class TaskPrintOne: public Task
{
public:
    virtual bool process()
    {
        std::cout << "One from " << pthread_self() << std::endl;
        return true;
    }
};

class TaskPrintTwo: public Task
{
public:
    virtual bool process()
    {
        std::cout << "Two from " << pthread_self() << std::endl;
        return true;
    }
};