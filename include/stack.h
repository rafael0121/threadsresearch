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

// Mymutex
#if mutex_type == 1

void lockmutex(void *);
void unlockmutex(void *);

#endif // end Mymutex

#endif // end Pthread mutex or MyMutex

/* mutex_type no defined */
#ifndef mutex_type 

#error mutex_type no defined.

#endif // end mutex_type no defined

#endif //end stack.h
