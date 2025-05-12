#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "llist.h"
#include "thread_pool.h"

#define MAX_THREADS 10

static LinkedList *task_queue = NULL;
static pthread_t threads[MAX_THREADS];
static pthread_cond_t task_queue_has_jobs = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t task_queue_rwmutex = PTHREAD_MUTEX_INITIALIZER;

// ========================= PROTOTYPES =========================

static void *thread_do();

// ========================= THREAD POOL ========================

// Initializes the thread pool and task queue
void thread_pool_init()
{
    // Initialize task queue
    task_queue = llist_create();

    // Initialize threads
    for (int i = 0; i < MAX_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, thread_do, NULL);
        pthread_detach(threads[i]); // Detach the thread to avoid calling join
    }
}

// Add new task to thread pool
void thread_pool_add_task(void (*function_p)(void *), void *arg_p)
{
    task_t *new_task = (task_t *)malloc(sizeof(task_t));

    new_task->func = function_p;
    new_task->arg = arg_p;

    // Lock the task queue before adding a new task
    pthread_mutex_lock(&task_queue_rwmutex);
    llist_push(task_queue, new_task);

    // Signal one thread that a new task is available
    pthread_cond_signal(&task_queue_has_jobs);

    pthread_mutex_unlock(&task_queue_rwmutex);
}

// ========================= THREAD =============================

// What each thread is doing
static void *thread_do()
{
    while (1)
    {
        pthread_mutex_lock(&task_queue_rwmutex);

        // Wait until there is an available task
        while (task_queue->size == 0)
            pthread_cond_wait(&task_queue_has_jobs, &task_queue_rwmutex);

        // Get next task from the queue
        Node *node = llist_pop(task_queue);

        if (node)
        {
            task_t *task = (task_t *)node->data;
            void (*task_func)(void *);
            void *arg;

            // Execute the task
            task_func = task->func;
            arg = task->arg;
            task_func(arg);

            free(task);
            free(node);
        }

        pthread_mutex_unlock(&task_queue_rwmutex);
    }

    return NULL;
}
