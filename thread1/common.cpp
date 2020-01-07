#include "common.h"
#include <pthread.h>

void LOG_ERROR(const char* msg)
{
    std::cout << msg << " from " << pthread_self() << std::endl;
}