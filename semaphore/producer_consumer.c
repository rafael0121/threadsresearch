#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <semaphore.h>
#include <pthread.h>

#define MAX 20000 
#define RAND 25

struct buffer{
    double *data;
    unsigned size;
    unsigned tail;
    unsigned head;
    sem_t sem;
    sem_t mutex;
    sem_t full;
    sem_t empty;
};

struct buffer * buffer_create()
{

    struct buffer *b = malloc(sizeof(struct buffer));
   
    b->data = malloc(sizeof(double) * MAX);
    b->size = MAX;
    b->head = 1;
    b->tail = 0;

    //init semaphore
    sem_init(&b->sem, 0, 1);
    sem_init(&b->full, 0, MAX-1);
    sem_init(&b->empty, 0, 0);
    sem_init(&b->mutex, 0, 1);

    return b;
}

void * push(void *obj, struct buffer *b)
{   
    if ((b->head) % b->size == b->tail) { 
        printf("NOT push\n");
        return NULL;
    }
    printf("push\n");
    double *obj_p = obj;
    void *ret = NULL;
    b->data[b->head] = *obj_p;
    ret = &b->data[b->head];
    b->head = (b->head + 1) % b->size;
    return ret;
}

void * pop(struct buffer *b)
{
    if ((b->tail + 1) % b->size == b->head) {
        printf("NOT pop\n");
        return NULL;
    }
    printf("pop\n");
    void *obj;
    b->tail = (b->tail + 1) % b->size;
    obj = &b->data[b->tail];
    return obj;
}

void * produce()
{
    double *c = malloc(sizeof(double));

    for (int i = 0; i < 9200; i++) {
    int count = 2 << (rand() % RAND);

    for (int i = 0; i < count; i++) {

        double a = 34.123412;
        double b = 3.22412345;

        *c = a / b * i;
    }

    void *obj = &c;
    
    return obj;
}

void consume(void *obj)
{
    double *c = obj;

    for (int i = 0; i < 9200; i++) {
    int count = 2 << (rand() % RAND);

    for (int i = 0; i < count; i++) {
        double a = 34.123412;
        double b = 34.123412;
        *c = a * b * i;
    }  
}

void * consumer(void *b)
{
    struct buffer *buf = b;
    while (true) {
        sem_wait(&buf->sem);
        void *obj = pop(buf);
        sem_post(&buf->sem);
    for (int i = 0; i < 1024; i++) {
        sem_wait(&buf->empty);
        sem_wait(&buf->mutex);
        void *obj = pop(buf);
        sem_post(&buf->mutex);
        sem_post(&buf->full);
        if (obj == NULL) {
            continue; 
        }
            consume(obj);
    }
}

void * producer(void *b)
{
    struct buffer *buf = b;
    void *ret;

    while (true) {
        void *obj = produce();
        if (obj != NULL) {
            sem_wait(&buf->sem);
            ret = push(obj, b);
            sem_post(&buf->sem);
    for (int i = 0; i < 1024; i++) {
        void *obj = produce();
        if (obj != NULL) {
            sem_wait(&buf->full);

            sem_wait(&buf->mutex);
            ret = push(obj, b);
            sem_post(&buf->mutex);

            sem_post(&buf->empty);
        }
    }
}

int main()
{
    srand(time(NULL));

    printf("Hello world!\n\n");
    
    pthread_t consumer_id, producer_id;

    int ret_consumer, ret_producer;
    
    void *b = buffer_create();
    
    pthread_attr_t attr;
    
    pthread_attr_init(&attr);

    pthread_create (&producer_id, &attr, &producer, b);
    pthread_create (&consumer_id, &attr, &consumer, b);
    
    ret_consumer = pthread_join(consumer_id, NULL);
    
    if (ret_consumer != 0) {
        printf("\nConsumer thread error\n");
        return -1;
    }

    ret_producer = pthread_join(producer_id, NULL);
     
    if (ret_producer != 0) {
        printf("\nProducer thread error\n");
        return -1;
    }

    return 0;
}



