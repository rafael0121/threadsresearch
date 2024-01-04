#ifndef STACK_H_
#define STACK_H_
    
    #include <stdlib.h>
    #include <semaphore.h>

    /* Buffer and operations */
    struct buffer {
        double *data;
        unsigned size;
        unsigned tail;
        unsigned head;
        sem_t full;     // Pause producer
        sem_t empty;    // Pause Consumer
        void *mutex;    // Control buffer access
    };
    struct buffer * buffer_create(int size);
    void * push (void *obj, struct buffer *b);
    void * pop (struct buffer *b);

/* Pthread mutex or MyMutex */
#ifdef mutex_type

// Pthread Mutex
#if mutex_type == 0

#include <pthread.h>

void * lockmutex(void *b) {
    pthread_mutex_lock((pthread_mutex_t) mutex);
}

void * unlockmutex(void *b) {
    pthread_mutex_unlock((pthread_mutex_t) mutex);
}

#endif // end Pthread Mutex

// Mymutex
#if mutex_type == 1
    
void lockmutex(void *);
void unlockmutex(void *);

#endif // end Mymutex

#else // if mutex_type no defined

#error mutex_type no defined.

#endif // end Pthread mutex or MyMutex

#endif //end stack.h
