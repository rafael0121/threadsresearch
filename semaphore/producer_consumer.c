#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <semaphore.h>
#include <pthread.h>

#define MAX 20000 
#define RAND 25

/* Buffer Struct */
struct buffer{
    double *data;  /* buffer */
    unsigned size; /* buffer size */
    unsigned tail; 
    unsigned head;
    sem_t mutex;   /* Control access in buffer. */
    sem_t full;    /* Pause producer */
    sem_t empty;   /* Pause consumer */
};

/**
 * Buffer Create
 */
struct buffer * buffer_create()
{

    struct buffer *b = malloc(sizeof(struct buffer));
   
    b->data = malloc(sizeof(double) * MAX);
    b->size = MAX;
    b->head = 1;
    b->tail = 0;

    //init semaphore
    sem_init(&b->full, 0, MAX-1);
    sem_init(&b->empty, 0, 0);
    sem_init(&b->mutex, 0, 1);

    return b;
}

void * push(void *obj, struct buffer *b)
{    
    void *ret = NULL;
    double *obj_p = obj;

    sem_wait(&b->full);
    sem_wait(&b->mutex);
    printf("push\n");
    b->data[b->head] = *obj_p;
    ret = &b->data[b->head];
    b->head = (b->head + 1) % b->size;

    sem_post(&b->mutex);
    sem_post(&b->empty);

    return ret;
}

void * pop(struct buffer *b)
{    
    void *obj;
    
    sem_wait(&b->empty);
    sem_wait(&b->mutex);

    printf("pop\n");
    b->tail = (b->tail + 1) % b->size;
    obj = &b->data[b->tail];

    sem_post(&b->mutex);
    sem_post(&b->full);

    return obj;
}

/**
 * @brief Produce obj
 */
void * produce()
{
    double *c = malloc(sizeof(double));

    int count = 2 << (rand() % RAND);

    for (int i = 0; i < count; i++) {

        double a = 34.123412;
        double b = 3.22412345;

        *c = a / b * i;
    }

    void *obj = &c;
    
    return obj;
}

/**
 * @brief Do operations in obj.
 */
void consume(void *obj)
{
    double *c = obj;

    int count = 2 << (rand() % RAND);

    for (int i = 0; i < count; i++) {
        double a = 34.123412;
        double b = 34.123412;
        *c = a * b * i;
    }  
}

/**
 * @brief Consumer thread.
 */
void * consumer(void *b)
{        
    struct buffer *buf = b;

    for (int i = 0; i < 1024; i++) {

        void *obj = pop(buf);

        if (obj == NULL) {
            continue; 
        }
            consume(obj);
    }
}

/**
 * @brief Producer thread.
 */
void * producer(void *b)
{
    struct buffer *buf = b;
    void *ret;

    for (int i = 0; i < 1024; i++) {
        void *obj = produce();
        if (obj != NULL) {
           ret = push(obj, b);
        }
    }
}

int main()
{
    srand(time(NULL));
    
    // Threads IDs. 
    pthread_t consumer_id, producer_id;
    
    // Receiver return threads.
    int ret_consumer, ret_producer;
    
    void *b = buffer_create();
    
    // Init default thread configurations. 
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    
    // Create threads.
    pthread_create (&producer_id, &attr, &producer, b);
    pthread_create (&consumer_id, &attr, &consumer, b);
    
    // Start Consumer thread.
    ret_consumer = pthread_join(consumer_id, NULL);
    
    if (ret_consumer != 0) {
        printf("\nConsumer thread error\n");
        return -1;
    }

    // Start Producer thread.
    ret_producer = pthread_join(producer_id, NULL);
     
    if (ret_producer != 0) {
        printf("\nProducer thread error\n");
        return -1;
    }

    return 0;
}



