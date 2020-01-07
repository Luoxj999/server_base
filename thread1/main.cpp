#include <unistd.h>
#include <iostream>
#include <pthread.h>

#include "task.h"
#include "threadpool.h"

int main()
{
    ThreadPool pool;
    pool.createThreadPool(2);

    std::cout << "start from " << pthread_self() << std::endl;

    sleep(1);

    Task* task1 = new TaskPrintOne;
    Task* task2 = new TaskPrintTwo;
    Task* task3 = new TaskPrintTwo;
    Task* task4 = new TaskPrintOne;
    Task* task5 = new TaskPrintTwo;
    Task* task6 = new TaskPrintOne;
    std::cout << "start add" << std::endl;
    pool.addTask(task1);
    pool.addTask(task2);
    pool.addTask(task3);
    pool.addTask(task4);
    pool.addTask(task5);
    pool.addTask(task6);

    sleep(5);

    pool.destroy();

    std::cout << "main finished." << std::endl;

    return 0;
}