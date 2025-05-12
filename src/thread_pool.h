#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

typedef struct
{
    void (*func)(void *arg); // Function pointer to the task function
    void *arg;               // Argument to the task function
} task_t;

/**
 * @brief Initializes thread pool
 */
void thread_pool_init();

/**
 * @brief Adds a new task to the thread pool
 *
 * @example
 *
 *    void print_num(int num){
 *       printf("%d\n", num);
 *    }
 *
 *    int main() {
 *       ..
 *       int a = 10;
 *       thread_pool_add_task((void*)print_num, (void*)a);
 *       ..
 *    }
 *
 *
 * @param function_p  Pointer to the function to be executed
 * @param arg_p       Pointer to the argument to be passed to the function
 */
void thread_pool_add_task(void (*function_p)(void *), void *arg_p);

#endif
