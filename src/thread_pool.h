#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

typedef struct
{
    void (*func)(void *arg); // Function pointer to the task function
    void *arg;               // Argument to the task function
} task_t;

void thread_pool_init();
void thread_pool_add_task(void (*function_p)(void *), void *arg_p);

void *thread_do();

#endif
