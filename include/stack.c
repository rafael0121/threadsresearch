#include "stack.h"

// Pthread Mutex
#if mutex_type == 0

#include <pthread.h>

void lockmutex (pthread_mutex_t *mutex) {
    pthread_mutex_lock(mutex);
}

void unlockmutex (pthread_mutex_t *mutex) {
    pthread_mutex_unlock(mutex);
}

#endif // end Pthread Mutex



struct buffer * buffer_create(int size)
{
    struct buffer *b = malloc(sizeof(struct buffer));
   
    b->data = malloc(sizeof(double) * size);
    b->size = size;
    b->head = 1;
    b->tail = 0;

    //init semaphore
    sem_init(&b->full, 0, size-1);
    sem_init(&b->empty, 0, 0);

#if mutex_type == 0
    //Init Mutex
    b->mutex = malloc(sizeof(pthread_mutex_t));

#endif

#if mutex_type == 1
    //Init Mymutex
    b->mutex = malloc(sizeof(int));
    b->mutex = 0;

#endif

    return b;
}

void * push(void *obj, struct buffer *b)
{    
    void *ret = NULL;
    double *obj_p = obj;

    sem_wait(&b->full);
    lockmutex(b->mutex);

    b->data[b->head] = *obj_p;
    ret = &b->data[b->head];
    b->head = (b->head + 1) % b->size;

    unlockmutex(b->mutex);
    sem_post(&b->empty);

    return ret;
}

void * pop(struct buffer *b)
{    
    void *obj;
    
    sem_wait(&b->empty);
    lockmutex(b->mutex);

    b->tail = (b->tail + 1) % b->size;
    obj = &b->data[b->tail];

    unlockmutex(b->mutex);
    sem_post(&b->full);

    return obj;
}

