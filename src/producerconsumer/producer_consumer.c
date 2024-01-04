#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#include <semaphore.h>
#include <pthread.h>

#include <stack.h>

#define ITEMS 1024
#define RAND 25
#define CLOCK_SEC 3200000000

typedef uint64_t CLOCK;

extern void getclock (CLOCK *);

void save_file(float timestamp){

    FILE *file = NULL;
    file = fopen("mymutex_results.csv", "a");
    assert(file != NULL);
    
    fprintf(file, "%f\n", timestamp);

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
    for (int i = 0; i < ITEMS; i++) {

        void *obj = pop(b);

        assert(obj != NULL);

        consume(obj);
    }

    return b;
}

/**
 * @brief Producer thread.
 */
void * producer(void *b)
{
    void *ret;

    for (int i = 0; i < ITEMS; i++) {
        void *obj = produce();
        if (obj != NULL) {
           ret = push(obj, b);
        }
        
        assert(ret != NULL);
    }
    
    return b;
}

int main()
{   
    srand(time(NULL));

    // Time stamp counter
    CLOCK t_start = 0;
    CLOCK t_end = 0;

    int size = 20;
    
    getclock(&t_start);

    // Threads IDs. 
    pthread_t consumer_id, producer_id;

    // Receiver return threads.
    int ret_consumer, ret_producer;
    
    void *b = buffer_create(size);
    
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

    getclock(&t_end);

    float timestamp = (float)(t_end - t_start) / 1600000000;

    save_file(timestamp);

    printf("Tempo de execução = %f", timestamp);

    return 0;
}



